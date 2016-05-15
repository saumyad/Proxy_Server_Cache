import scipy
import numpy as np
import pandas as pd
import csv
import matplotlib.pyplot as plt
import pylab as P
from scipy import stats
import math
from scipy.optimize import minimize
from sklearn.feature_extraction import DictVectorizer
from pyfm import pylibfm
from sklearn.metrics import mean_squared_error
import random

n_s = 50
n_l = 200
file_name_s_u = 'testset_small_uniform.txt'
file_name_s_r = 'testset_small_random.txt'
file_name_l_u = 'testset_large_uniform.txt'
file_name_l_r = 'testset_large_random.txt'

names= ['site', 'size']

df = pd.read_csv("webdata_small.txt", delimiter=',', names = names, skiprows=1)
l = len(df.index)

x = np.random.uniform(0, l, n_s)
res_s_u = pd.DataFrame(columns=names)
res_s_u = res_s_u.fillna(0)
res_s_u = res_s_u.astype(int)
for i in x:
	j = int(i)
	df2 = pd.DataFrame([[df.ix[j]['site'], int(df.ix[j]['size'])]], columns=names)
	res_s_u = res_s_u.append(df2)
res_s_u.to_csv(file_name_s_u, sep=' ', index=None, header=False, quoting=csv.QUOTE_NONE)

x = np.random.randint(0, l, n_s)
res_s_r = pd.DataFrame(columns=names)
res_s_r = res_s_r.fillna(0)
res_s_r = res_s_r.astype(int)
for i in x:
	j = int(i)
	df2 = pd.DataFrame([[df.ix[j]['site'], int(df.ix[j]['size'])]], columns=names)
	res_s_r = res_s_r.append(df2)
res_s_r.to_csv(file_name_s_r, sep=' ', index=None, header=False, quoting=csv.QUOTE_NONE)

x = np.random.uniform(0, l, n_l)
res_l_u = pd.DataFrame(columns=names)
res_l_u = res_l_u.fillna(0)
res_l_u = res_l_u.astype(int)
for i in x:
	j = int(i)
	df2 = pd.DataFrame([[df.ix[j]['site'], int(df.ix[j]['size'])]], columns=names)
	res_l_u = res_l_u.append(df2)
res_l_u.to_csv(file_name_l_u, sep=' ', index=None, header=False, quoting=csv.QUOTE_NONE)

x = np.random.randint(0, l, n_l)
res_l_r = pd.DataFrame(columns=names)
res_l_r = res_l_r.fillna(0)
res_l_r = res_l_r.astype(int)
for i in x:
	j = int(i)
	df2 = pd.DataFrame([[df.ix[j]['site'], int(df.ix[j]['size'])]], columns=names)
	res_l_r = res_l_r.append(df2)
res_l_r.to_csv(file_name_l_r, sep=' ', index=None, header=False, quoting=csv.QUOTE_NONE)