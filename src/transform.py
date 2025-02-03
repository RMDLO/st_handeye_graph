from pyquaternion import Quaternion
import numpy as np

t1 = [0, 0, 0]
q1 = Quaternion(np.array([-0.5, 0.5, -0.5, 0.5])) # w, x, y, z
T1 = np.zeros((4,4))
R1 = q1.rotation_matrix
T1[0:3, 0:3] = R1
T1[0:3, 3] = t1
T1[3,3] = 1

t2 = [-0.348229, 0.482274, 0.547362]
# from st_handeye_graph
R2 = np.array([[-0.989659, -0.0281278, -0.140652],[0.0736322, 0.741898, -0.666457],[0.123096, -0.669922, -0.732156]])
# Perform SVD decomposition
U, _, Vt = np.linalg.svd(R2)
# Reconstruct the orthogonal matrix
R_orthogonal = np.dot(U, Vt)
q2 = Quaternion(matrix=R_orthogonal)
# q2 = Quaternion(np.array([-0.0382, -0.0456, 0.9259, -0.3731])) # w, x, y, z
T2 = np.zeros((4,4))
R2 = q2.rotation_matrix
T2[0:3, 0:3] = R2
T2[0:3, 3] = t2
T2[3,3] = 1

T = T2@np.linalg.inv(T1)
static_tf = list(T[0:3,3])
q = Quaternion(matrix=T)
q_elements = list(q.elements)

for qi in q_elements[1:]:
	static_tf.append(qi)
static_tf.append(q_elements[0])

print(static_tf)
