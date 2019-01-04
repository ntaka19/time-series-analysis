import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

df = pd.read_csv(r'northwind20041025130835.csv',header=None)
t=np.arange(0,len(df.values))
f=np.array(df.values,dtype='float').flatten()


def d(series,i,j):
    return abs(series[i]-series[j])

series=f
N=len(series)
#eps=input('Initial diameter bound: ')
eps=0.01
dlist=[[] for i in range(N)]
n=0 #number of nearby pairs found

initialpoints=np.array([i*100 for i in range(int(N/100))])
#初期点に対し，十分近い点を持ってくる．
#それらの距離をステップ毎に調べていく．
#for i in range(N):
for num,i in enumerate(initialpoints):
    if num%100==0:
        print(i)
    
    #for j in range(i+1,N):
    for j in range(i+1,i+1000):
        if d(series,i,j) < eps:
            #for k in range(min(N-i,N-j)):
            for k in range(10000):
                dlist[k].append(np.log( max(d(series,i+k,j+k), 0.0001) ))


lyp=[]
xx=[]
for i in range(len(dlist)):
    if len(dlist[i])>0:
        #np.append(lyp,sum(dlist[i])/len(dlist[i]))
        lyp.append(sum(dlist[i])/len(dlist[i]))
        xx.append(i)


plt.figure()
plt.plot(xx,lyp)
plt.show()

#f=open('lyapunov.txt','w')
#for i in range(len(dlist)):
#    if len(dlist[i]):
#        print>>f, i, sum(dlist[i])/len(dlist[i])
#f.close()
