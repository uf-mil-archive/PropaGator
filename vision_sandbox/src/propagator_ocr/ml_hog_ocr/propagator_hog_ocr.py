__author__ = 'Ralph F. Leyva'

import cv2
import numpy as np
from sklearn import preprocessing
from skimage.feature import hog
from sklearn.svm import LinearSVC
from sklearn import svm
from sklearn.externals import joblib
from sklearn.neighbors import KNeighborsClassifier

class dataset():
    def __init__(self):
        self.samples = np.genfromtxt('final_samp.data').astype(np.float32)
        self.responses = np.genfromtxt('final_resp.data').astype(np.float32)

digit_repo = dataset()
digits = np.array(digit_repo.samples, 'float32')
labels = np.array(digit_repo.responses, 'float32')

list_hog_fd = []
list_hog_images = []
list_digit = []

for digit in digits:
    fd, hog_image = hog(digit.reshape(30,30), orientations=10, pixels_per_cell=(3,3), cells_per_block=(1, 1), visualise=True)
    list_hog_fd.append(fd)
    list_hog_images.append(hog_image)

hog_features = np.array(list_hog_fd, 'float32')
#hog_features = preprocessing.normalize(hog_features, norm='l1')

cv2.imshow('', list_hog_images[540])
cv2.waitKey(0)
cv2.imshow('', list_hog_images[1200])
cv2.waitKey(0)
cv2.imshow('', list_hog_images[700])
cv2.waitKey(0)

#clf = svm.SVC(kernel='poly',degree=3)
clf = KNeighborsClassifier(n_neighbors=10, weights='distance')
clf.fit(hog_features, labels)
joblib.dump(clf, "digits_cls.pkl", compress=3)