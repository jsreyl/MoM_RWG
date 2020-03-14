import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import sys

#print(int(sys.argv[1]))
#arguments should be this_script.py ProblemType L W quad_elems

#read the dataframes
dfAA=pd.read_csv(r"AA_cut.csv")#has columns y_vert and InormAA
dfBB=pd.read_csv(r"BB_cut.csv")#has columns x_hor and InormBB
L=float(sys.argv[2])
W=float(sys.argv[3])

if int(sys.argv[1])==0:
    ref_data_x_AA=L*np.linspace(1./10, 9./10, 5)
    ref_data_x_BB=W*np.linspace(1./6, 5./6, 5)
    ref_data_y_AA=[1.8, 1.1, 1.1, 1.1, 1.8]
    ref_data_y_BB=[0.8,1.,1.1,1.,0.8]
    max_y=3
elif int(sys.argv[1])==1:
    ref_data_x_AA=L*np.linspace(1./14, 13./14, 7)
    ref_data_x_BB=W*np.linspace(1./6, 5./6, 5)
    ref_data_y_AA=[4.6, 2.6, 2.8, 2.9, 2.8, 2.6, 4.6]
    ref_data_y_BB=[1.7, 2.5, 2.9, 2.5, 1.7]
    max_y=6
elif int(sys.argv[1])==2:
    ref_data_x_AA=L*np.linspace(1./12, 11./12, 6)
    ref_data_x_BB=W*np.linspace(1./12, 11./12, 6)
    ref_data_y_AA=[0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
    ref_data_y_BB=[4.5, 2.7, 2.7, 2.8, 2.7,4.0]
    max_y=6

if dfAA is not None and dfBB is not None:
    plt.plot(dfAA['y_vert'],dfAA['InormAA'],'xb', label='Cut AA with '+sys.argv[4]+' quad pts')
    plt.plot(ref_data_x_AA,ref_data_y_AA,'-ob', label='Cut AA [RWG]')
    plt.plot(dfBB['x_hor'],dfBB['InormBB'],'+r', label='Cut BB with '+sys.argv[4]+' quad pts')
    plt.plot(ref_data_x_BB,ref_data_y_BB,'--sr', label='Cut BB [RWG]')

    plt.legend(loc='upper right')
    plt.xlabel(r'X/$\lambda$, Y/$\lambda$')
    plt.ylabel(r'$|\frac{J_x}{H_{inc}}|$')
    if int(sys.argv[1])==0:
        plt.savefig('IvsX_0.pdf')
    if int(sys.argv[1])==1:
        plt.savefig('IvsX_1.pdf')
    if int(sys.argv[1])==2:
        plt.savefig('IvsX_2.pdf')
    plt.show()

