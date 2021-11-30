import numpy as np
from scipy import sparse
from random import shuffle as sl  
from random import randint as rd  
import sys  
 
def random_graph(node,edge):

    n = node
    node = range(0,n)   
    node = list(node)  
    
    sl(node)  
    m = edge
    result = [] 
    
    appeared_node = []
    not_appeared_node = node
 
    while len(result) != n - 1:

        if len(result) == 0:
            p1 = rd(0,n - 2)
            p2 = rd(p1+1,n - 1)
            x = node[p1]
            y = node[p2]
            appeared_node.append(x)
            appeared_node.append(y)
            not_appeared_node = list(set(node).difference(set(appeared_node)))
            result.append((x,y))

        else:
            p1 = rd(0,len(appeared_node) - 1)
            x = appeared_node[p1]
            p2 = rd(0,len(not_appeared_node) - 1)
            y = not_appeared_node[p2]
            appeared_node.append(y)
            not_appeared_node = list(set(node).difference(set(appeared_node)))
    
            if node.index(y) < node.index(x):
                result.append((y,x))
            else:
                result.append((x,y))
      
    while len(result) != m:
        p1 = rd(0,n - 2)
        p2 = rd(p1+1,n - 1)
        x = node[p1]
        y = node[p2]
   
        if (x,y) in result:
            continue
        else:
            result.append((x,y))
    
    matrix = np.zeros((n,n))
    for i in range(len(result)):
        matrix[result[i][0],result[i][1]] = rd(1, 100)

    return matrix


N = 512
edge = 1000
if len(sys.argv) == 2:
    N = int(sys.argv[1])
if len(sys.argv) == 3:
    N = int(sys.argv[1])
    edge = int(sys.argv[2])


G1 = random_graph(N,edge)
sA = sparse.csr_matrix(G1)

f = open('v' + str(N) + '_e' + str(edge) + '.txt', 'w') 
f.write(str(N) + "\n" + str(edge) + "\n")

# f.write(str(len(sA.data)) + "\n")
np.savetxt(f, sA.data, fmt='%d', delimiter = ',')
# f.write(str(len(sA.indices)) + "\n")
np.savetxt(f, sA.indices, fmt='%d', delimiter = ',')
# f.write(str(len(sA.indptr)) + "\n")
np.savetxt(f, sA.indptr, fmt='%d', delimiter = ',')

#print(G1)
#print(sA)
