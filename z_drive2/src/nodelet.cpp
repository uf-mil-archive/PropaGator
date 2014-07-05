#include <boost/bind.hpp>
#include <boost/foreach.hpp>

#include <nodelet/nodelet.h>
#include <pluginlib/class_list_macros.h>
#include <ros/ros.h>
#include <tf_conversions/tf_eigen.h>

#include <odom_estimator/util.h>

using odom_estimator::Vec;
using odom_estimator::SqMat;
using odom_estimator::Quaternion;
using odom_estimator::quat_from_rotvec;

namespace z_drive2 {

template<typename T>
std::vector<T> concat(std::vector<T> const & a, std::vector<T> const & b) {
  std::vector<T> res = a;
  res.insert(res.end(), b.begin(), b.end());
  return res;
}

struct Parameters {
  double mass;
  SqMat<3> moment_of_inertia;
  struct Thruster {
    Vec<3> position;
    Quaternion orientation;
    double angle_min, angle_max;
    double dangle_min, dangle_max;
    double thrust_min, thrust_max;
    double dthrust_min, dthrust_max;
  };
  std::vector<Thruster> thrusters;
};

struct Control {
  struct Thruster {
    double dangle;
    double dthrust;
    Thruster(double dangle, double dthrust) : dangle(dangle), dthrust(dthrust) { }
  };
  std::vector<Thruster> thrusters;
  Control(std::vector<Thruster> const & thrusters) : thrusters(thrusters) { }
  static Control Zero(Parameters const & p) {
    return Control(std::vector<Thruster>(p.thrusters.size(), Thruster(0, 0)));
  }
  static std::vector<Control> SpanningSet(Parameters const & p) {
    std::vector<Control> res;
    res.push_back(Control(std::vector<Thruster>()));
    
    for(unsigned int i = 0; i < p.thrusters.size(); i++) {
      std::vector<Control> new_res;
      BOOST_FOREACH(Control const & ee, res) {
        std::vector<Thruster> const & e = ee.thrusters;
        new_res.push_back(Control(concat(e, std::vector<Thruster>{Thruster(p.thrusters[i].dangle_min, p.thrusters[i].dthrust_min)})));
        new_res.push_back(Control(concat(e, std::vector<Thruster>{Thruster(p.thrusters[i].dangle_min, p.thrusters[i].dthrust_max)})));
        new_res.push_back(Control(concat(e, std::vector<Thruster>{Thruster(p.thrusters[i].dangle_max, p.thrusters[i].dthrust_min)})));
        new_res.push_back(Control(concat(e, std::vector<Thruster>{Thruster(p.thrusters[i].dangle_max, p.thrusters[i].dthrust_max)})));
      }
      res = std::move(new_res);
    }
    
    return res;
  }
  static std::vector<std::vector<Control> > SmallSpanningSet(Parameters const & p) {
    std::vector<std::vector<Control> > res;
    for(unsigned int i = 0; i < p.thrusters.size(); i++) {
      Control a = Zero(p); a.thrusters[i].dangle = p.thrusters[i].dangle_min;
      Control b = Zero(p); b.thrusters[i].dangle = p.thrusters[i].dangle_max;
      res.push_back(std::vector<Control>{a, b});
      Control c = Zero(p); c.thrusters[i].dthrust = p.thrusters[i].dthrust_min;
      Control d = Zero(p); d.thrusters[i].dthrust = p.thrusters[i].dthrust_max;
      res.push_back(std::vector<Control>{c, d});
    }
    return res;
  }
  Control operator+(Control const & other) const {
    std::vector<Thruster> x;
    assert(thrusters.size() == other.thrusters.size());
    for(unsigned int i = 0; i < thrusters.size(); i++) {
      Thruster const & ours = thrusters[i]; Thruster const & theirs = other.thrusters[i];
      x.push_back(Thruster(ours.dangle + theirs.dangle, ours.dthrust + theirs.dthrust));
    }
    return Control(x);
  }
};

struct State {
  Vec<3> position;
  Quaternion orientation;
  Vec<3> velocity;
  Vec<3> angular_velocity;
  struct Thruster {
    double angle;
    double thrust;
    Thruster(double angle, double thrust) : angle(angle), thrust(thrust) { }
    Thruster update(Control::Thruster const & control, double dt, const Parameters::Thruster & p) const {
      return Thruster(angle + control.dangle * dt, thrust + control.dthrust * dt);
    }
  };
  std::vector<Thruster> thrusters;
  State(
      Vec<3> position,
      Quaternion orientation,
      Vec<3> velocity,
      Vec<3> angular_velocity,
      std::vector<Thruster> thrusters) :
    position(position),
    orientation(orientation),
    velocity(velocity),
    angular_velocity(angular_velocity),
    thrusters(thrusters) {
  }
  
  bool thrusts_approximately_zero() const {
    for(unsigned int i = 0; i < thrusters.size(); i++) {
      if(fabs(thrusters[i].thrust) > 1e-6) {
        return false;
      }
    }
    return true;
  }
  
  State update(Control const & control, double dt, const Parameters & p) const {
    assert(thrusters.size() == p.thrusters.size());
    
    std::vector<Thruster> res_thrusters;
    for(unsigned int i = 0; i < thrusters.size(); i++) {
      res_thrusters.push_back(thrusters[i].update(control.thrusters[i], dt, p.thrusters[i]));
    }
    
    Vec<3> force_body = Vec<3>::Zero();
    Vec<3> torque_body = Vec<3>::Zero();
    for(unsigned int i = 0; i < thrusters.size(); i++) {
      State::Thruster const & ts = res_thrusters[i];
      Parameters::Thruster const & tp = p.thrusters[i];
      
      Vec<3> thrust_thruster = ts.thrust * Vec<3>(cos(ts.angle), sin(ts.angle), 0);
      Vec<3> thrust_body = tp.orientation._transformVector(thrust_thruster);
      
      force_body += thrust_body;
      torque_body += tp.position.cross(thrust_body);
    }
    
    Vec<3> acceleration = orientation._transformVector(1/p.mass * force_body);
    Vec<3> angular_acceleration = orientation._transformVector(p.moment_of_inertia.inverse() * torque_body);
    
    return State(
      position + velocity * dt + acceleration * pow(dt, 2)/2,
      quat_from_rotvec(angular_velocity * dt + angular_acceleration * pow(dt, 2)/2) * orientation,
      velocity + acceleration * dt,
      angular_velocity + angular_acceleration * dt,
      res_thrusters);
  }
};


Control compute_control_policy(State const & state, double dt, Parameters const & p) {
  // compute control to bring thrust to 0 as fast as possible
  std::vector<Control::Thruster> res;
  for(unsigned int i = 0; i < state.thrusters.size(); i++) {
    double desired_thrust_change = -state.thrusters[i].thrust;
    double dthrust = desired_thrust_change < 0 ? p.thrusters[i].dthrust_min : p.thrusters[i].dthrust_max;
    if(fabs(dthrust * dt) > fabs(desired_thrust_change)) {
      dthrust = desired_thrust_change / dt;
    }
    res.push_back(Control::Thruster(0, dthrust));
  }
  return Control(res);
}

Control compute_vel_policy(State const & state, double dt, Parameters const & p) {
  // compute control to bring velocity to 0 as fast as possible, with thrust being 0 at that point
  Control res = Control::Zero(p);
  
  BOOST_FOREACH(std::vector<Control> const & controls, Control::SmallSpanningSet(p)) {
    boost::optional<std::pair<double, Control> > best;
    BOOST_FOREACH(Control const & control, controls) {
      // simulate final velocity of trajectory taken while bringing thrust down to 0
      // then determine control now that will result in final velocity being closer to 0
      State s = state.update(control, dt, p);
      while(!s.thrusts_approximately_zero()) {
        s = s.update(compute_control_policy(s, dt, p), dt, p);
      }
      
      double error = std::max(s.velocity.norm(), s.angular_velocity.norm());
      
      if(!best || error < best->first) {
        best = std::make_pair(error, control);
      }
    }
    res = res + best->second;
  }
  
  return res;
}

Control compute_pos_policy(State const & state, Vec<3> desired_pos, double dt, Parameters const & p) {
  boost::optional<std::pair<double, Control> > res;
  
  BOOST_FOREACH(Control const & control, Control::SpanningSet(p)) {
    //std::cout << std::endl;
    State s = state.update(control, dt, p);
    while(s.velocity.norm() >= 1e-2 || !s.thrusts_approximately_zero()) {
      Control a = compute_vel_policy(s, dt, p);
      /*std::cout << "  Thruster states:" << std::endl;
      BOOST_FOREACH(State::Thruster const & thruster, s.thrusters) {
        std::cout << "  " << thruster.angle << " " << thruster.thrust << std::endl;
      }
      std::cout << "  Position: " << s.position.transpose() << std::endl;
      std::cout << "  Velocity: " << s.orientation.inverse()._transformVector(s.velocity).transpose() << std::endl;
      std::cout << "  Angular velocity: " << s.angular_velocity.transpose() << std::endl;
      std::cout << "  Thruster commands:" << std::endl;
      BOOST_FOREACH(Control::Thruster const & thruster, a.thrusters) {
        std::cout << "  " << thruster.dangle << " " << thruster.dthrust << std::endl;
      }
      std::cout << std::endl;*/
      s = s.update(a, dt, p);
    }
    
    double error = (s.position - desired_pos).norm();
    
    if(!res || error < res->first) {
      res = std::make_pair(error, control);
    }
  }
  
  return res->second;
}

class NodeImpl {
private:
  boost::function<const std::string&()> getName;
  ros::NodeHandle &nh;
  ros::NodeHandle &private_nh;
public:
  NodeImpl(boost::function<const std::string&()> getName, ros::NodeHandle *nh_, ros::NodeHandle *private_nh_) :
    getName(getName),
    nh(*nh_),
    private_nh(*private_nh_)  {
    
    // XXX
    Parameters p;
    p.mass = 40;
    p.moment_of_inertia <<
      40, 0, 0,
      0, 40, 0,
      0, 0, 40;
    {
      Parameters::Thruster t;
      t.orientation = Quaternion::Identity();
      t.angle_min = -1.6; t.angle_max = +1.6;
      t.dangle_min = -3.2; t.dangle_max = +3.2;
      t.thrust_min = -23.65; t.thrust_max = 39.25;
      t.dthrust_min = -100; t.dthrust_max = +100;
      
      t.position = Vec<3>(-1, +1, 0);
      p.thrusters.push_back(t);
      t.position = Vec<3>(-1, -1, 0);
      p.thrusters.push_back(t);
    }
    
    State s(
      Vec<3>::Zero(),
      Quaternion::Identity(),
      Vec<3>(0, 0, 0),
      Vec<3>::Zero(),
      std::vector<State::Thruster>{
        State::Thruster(0, 0),
        State::Thruster(0, 0)});
    
    double dt = 1/20.;
    
    double t = 0;
    while(true) {
      std::cout << std::endl;
      std::cout << "Time: " << t << std::endl;
      std::cout << "Thruster states:" << std::endl;
      BOOST_FOREACH(State::Thruster const & thruster, s.thrusters) {
        std::cout << thruster.angle << " " << thruster.thrust << std::endl;
      }
      std::cout << "Position: " << s.position.transpose() << std::endl;
      std::cout << "Velocity: " << s.velocity.transpose() << std::endl;
      std::cout << "Angular velocity: " << s.angular_velocity.transpose() << std::endl;
      Control a = compute_pos_policy(s, Vec<3>(1, 0, 0), dt, p);
      //Control a = compute_vel_policy(s, dt, p);
      std::cout << "Thruster commands:" << std::endl;
      BOOST_FOREACH(Control::Thruster const & thruster, a.thrusters) {
        std::cout << thruster.dangle << " " << thruster.dthrust << std::endl;
      }
      s = s.update(a, dt, p);
      t += dt;
      std::cout << std::endl;
    }
  }
};

class Nodelet : public nodelet::Nodelet {
private:
  boost::optional<NodeImpl> nodeimpl;

public:
  Nodelet() {
  }
  void onInit() {
    nodeimpl = boost::in_place(boost::bind(&Nodelet::getName, this),
      &getNodeHandle(), &getPrivateNodeHandle());
  }
};
PLUGINLIB_EXPORT_CLASS(z_drive2::Nodelet, nodelet::Nodelet);

}
