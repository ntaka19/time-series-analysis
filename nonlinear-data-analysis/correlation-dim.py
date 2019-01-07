import pandas as pd
import numpy as np
import matplotlib.pyplot as plt


def cordim(N,m,f):
    count=0
    Clist=[]
#M=len(xx)
    #xx=np.array([np.array([ f[m*i+j] for j in range(m)] ) for i in range(int(N/m)) ])

    tau=10
    xx=np.array([np.array([ f[i+tau*j] for j in range(m)] ) for i in range(N-m*tau) ])

    M=1000
    rlist=[0.005,0.01, 0.02,0.03,0.04,0.05,0.06,0.07,0.08,0.09 ,0.1,0.15 ,0.2, 0.5,1.0,1.5,2.0]

    for r in rlist:
        count=0
        for i in range(M):
            if(i%100==0):
                print(i)
            x1=xx[i]
            for j in range(M):
                x2=xx[j]
                if(  np.sum(abs(x1-x2)) <r):
                    count+=1

        Clist.append(count/pow(N,2))

    return rlist,Clist



def savefig(m,rlist,Clist):
    plt.figure()
    plt.plot(np.log10(rlist), np.log10(Clist), 'bo')
    plt.plot(np.log10(rlist), np.log10(rlist)*0.9-4 , label="0.9x-4.0")
    plt.xlabel("log(r)")
    plt.ylabel("log C(r)")
    plt.legend()
    plt.title("Correlation Dimension")
    plt.savefig( 'cordim-dim{dim}.png'.format(dim=m))
    plt.close()



def calculate():
    df = pd.read_csv(r'northwind20041025130835.csv',header=None)
    t=np.arange(0,len(df.values))
    f=np.array(df.values,dtype='float').flatten()
    N=len(df)
    for m in [3]:
        rlist,Clist=cordim(N,m,f)
        savefig(m,rlist,Clist)
        
    
if __name__ == '__main__':
    calculate()


