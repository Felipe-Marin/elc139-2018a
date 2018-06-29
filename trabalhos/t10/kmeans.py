"""
@author:Felipe
"""
import pandas as pd
import numpy as np
import random

def initCentroids(matrix, n_clusters):
    centroids = []
    for i in range(0, n_clusters):
        centroids.append(matrix[random.randint(0, matrix.shape[0]-1)])
    return np.array(centroids)

def nearestCentroid(row, centroids):
    distances = []
    for centroid in centroids:
        distances.append(np.linalg.norm(row-centroid))
    return np.argmin(distances)

def getLabels(matrix, centroids):
    labels = []
    for row in matrix:
        labels.append(nearestCentroid(row, centroids))
    return np.array(labels)

def getCentroids(matrix, centroids, labels):
    for centroid in centroids:
        centroid = [0] * centroid.shape[0]
    centroidsCount = [0] * centroids.shape[0]
    for i in range(0, matrix.shape[0]):
        centroids[labels[i]] = np.add(centroids[labels[i]], matrix[i])
        centroidsCount[labels[i]] += 1
    for i in range(0, centroids.shape[0]):
        centroids[i] = np.divide(centroids[i], centroidsCount[i])
    return centroids

def kmeans(matrix, k, maxIterations):
    centroids = initCentroids(matrix, k)
    oldCentroids = None
    iterations = 0
    while (not np.array_equal(centroids, oldCentroids)) and iterations < maxIterations:
        oldCentroids = centroids
        labels = getLabels(matrix, centroids)
        centroids = getCentroids(matrix, centroids, labels)
        iterations += 1
    return labels

#número de clusters
k = 2
#máximo de iterações
maxIterations = 300
#arquivo csv com os dados
input = pd.read_csv('datasets/Watch_gyroscope.csv')
#colunas do arquivo a serem usadas no kmeans
columns = ['x', 'y', 'z']
#nome do arquivo de saida
output = 'kmeans.csv'

X = input.as_matrix(columns)
labels = kmeans(X, k, maxIterations)
df = pd.DataFrame(data=X, columns=columns)
df['labels'] = labels
df.to_csv(output)