import pycuda.gpuarray as gpuarray
import pycuda.driver as cuda
import pycuda.autoinit
import pycuda.cumath as cumath
import numpy
import random
import pandas as pd
import time

from pycuda.compiler import SourceModule

mod = SourceModule("""
__global__ void getLabels(float *matrix, float *centroids, int *labels, int shapeX, int shapeY, int k)
{
    int row = (blockIdx.x * blockDim.x * blockDim.y + blockIdx.y * blockDim.x * blockDim.y * gridDim.x +  threadIdx.x + threadIdx.y * blockDim.x);
    while(row < shapeY){
        float minDistance = -1.0;
        int label = 0;
        for(int j = 0; j < k; j++){
            float sum = 0.0f;
            for(int i = 1; i<shapeX;i++){
                //printf("row:%d, index:%f, i:%d, value:%f\\n",row, matrix[row*shapeX], i, matrix[row*shapeX+i]);
                sum += powf(matrix[row*shapeX+i]-centroids[j*(shapeX-1)+i-1],2);
            }
            sum = sqrtf(sum);
            //printf("row:%d, minDistance: %f , sum=%f, label=%d, j=%d\\n", row, minDistance, sum, label, j);
            if(minDistance < 0 || sum < minDistance){
                minDistance = sum;
                label = j;
            }
            //printf(" - row=%d, label=%d, minDistance=%f, sum=%f, k=%d - ", row, label, minDistance, sum, j);
        }
        labels[row*2] = matrix[row*shapeX];
        labels[row*2+1] = label;
        row += (gridDim.x*blockDim.x)*(gridDim.y*blockDim.y); 
    }
} 
""")

def initCentroids(matrix, n_clusters):
    centroids = []
    for i in range(0, n_clusters):
        centroids.append(matrix[random.randint(0, matrix.shape[0])-1, 1:])
    return numpy.array(centroids)

def getCentroids(matrix, centroids, labels):
    centroids = numpy.zeros(centroids.shape)
    centroidsCount = numpy.zeros(centroids.shape[0])
    for i in range(0, matrix.shape[0]):
        centroids[labels[i,1]] = numpy.add(centroids[labels[i,1]], matrix[i,1:])
        centroidsCount[labels[i,1]] += 1
    for i in range(0, centroids.shape[0]):
        centroids[i] = numpy.divide(centroids[i], centroidsCount[i])
    return centroids

def kmeans(matrix, k, maxIterations):
    centroids = initCentroids(matrix, k)
    oldCentroids = None
    iterations = 0
    matrix_gpu = cuda.mem_alloc(matrix.nbytes)
    cuda.memcpy_htod(matrix_gpu, matrix)
    while (not numpy.array_equal(centroids, oldCentroids)) and iterations < maxIterations:
        oldCentroids = centroids
        centroids = numpy.ascontiguousarray(centroids, dtype=numpy.float32)
        labels = numpy.ascontiguousarray(numpy.empty((matrix.shape[0], 2)), dtype=numpy.int32)
        func = mod.get_function("getLabels")
        func(matrix_gpu, cuda.In(centroids), cuda.InOut(labels), numpy.int32(matrix.shape[1]), numpy.int32(matrix.shape[0]), numpy.int32(k), grid=(6,10,1), block=(32,32,1))
        centroids = getCentroids(matrix, centroids, labels)
        iterations += 1
    print(iterations)
    return labels

#número de clusters
k = 300
#máximo de iterações
maxIterations = 5
#colunas do arquivo a serem usadas no kmeans
columns = ['Index', 'x', 'y', 'z']
#arquivo csv com os dados
input = pd.read_csv('datasets/Watch_gyroscope.csv', usecols=columns)
#nome do arquivo de saida
output = 'kmeans.csv'

X = numpy.ascontiguousarray(input.values, dtype=numpy.float32)
start_time = time.perf_counter()
labels = kmeans(X, k, maxIterations)
end_time = time.perf_counter()
print("Kmeans time:" + str(end_time-start_time))
df = pd.DataFrame(data=X, columns=columns)
df['labels'] = labels[:,1]
df.to_csv(output)