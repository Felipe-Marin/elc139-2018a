# -*- coding: utf-8 -*-
"""
Created on Sun Jun 11 17:54:18 2017

@author: Felipe
"""

import pandas as pd
import numpy as np
import matplotlib.pyplot as pl
from sklearn.cluster import KMeans
import time

def kmeans_predict(X, n_cluster):
    kmeans = KMeans(n_clusters=n_cluster, n_init=1, max_iter=3, n_jobs=1).fit(X)
    return kmeans.labels_

##def gera_grafico(lista, nome):
##    pl.plot = lista
##    pl.title = nome
##    pl.show()    

k = 300
columns = ['x', 'y', 'z']
input = pd.read_csv('datasets/Watch_gyroscope.csv', usecols=columns)
X = input.values
start_time = time.perf_counter()
labels = kmeans_predict(X, k)
end_time = time.perf_counter()
print("Kmeans time:" + str(end_time-start_time))
df = pd.DataFrame()
df['labels'] = labels
df.to_csv('clusters.csv')