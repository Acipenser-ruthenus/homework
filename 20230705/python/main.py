import pandas as pd
import os
from ctypes import *
import math

# 加载dll
cdll = CDLL("D:/homework/20230705/CTest2/Project1/x64/Release/setDLL.dll", winmode=0)
WinDLL("D:/homework/20230705/CTest2/Project1/x64/Release/setDLL.dll", winmode=0)

# 读取csv文件
data0 = pd.read_csv("C:/Users/U/Desktop/1(1).csv")
print(data0.head())
C_x_values = data0['x']
C_y_values = data0['y']
C_type_values = data0['type']
valueSize = len(C_type_values)
C_x_values = C_x_values.astype(int).tolist()
C_y_values = C_y_values.astype(int).tolist()
C_type_values = C_type_values.astype(int).tolist()

# 创建用于传递数据的ctypes数组
C_x_values = (c_int * len(C_x_values))(*C_x_values)
C_y_values = (c_int * len(C_y_values))(*C_y_values)
C_type_values = (c_int * len(C_type_values))(*C_type_values)

# 绘制散点图
# cdll.drawScatter(C_x_values, C_y_values, C_type_values, valueSize)

# 绘制y轴直方图
histSize = math.ceil(max(C_y_values)-min(C_y_values))
C_out_hist_y = ((c_int) * histSize)()
cdll.CalcHist(C_x_values, C_y_values, C_out_hist_y, valueSize, histSize, 0)

# 滤波
C_out_hist1_y = ((c_int) * histSize)()
cdll.MedianBlur(C_out_hist_y, C_out_hist1_y, histSize, 3)
cdll.MeanBlur(C_out_hist1_y, C_out_hist_y, histSize, 3)
cdll.MeanBlur(C_out_hist_y, C_out_hist1_y, histSize, 3)
# cdll.drawHistogramFliter(C_out_hist1_y, histSize)

# 旋转
angle = 63.4
dis = max(math.sin(math.radians(angle)),math.cos(math.radians(angle)))
angle = c_double(angle)
is_x = 0

in_hist_x_rotated = ((c_double) * valueSize)()
in_hist_y_rotated = ((c_double) * valueSize)()


cdll.rotatePointsX(C_x_values, C_y_values, valueSize, angle, in_hist_x_rotated)
cdll.rotatePointsY(C_x_values, C_y_values, valueSize, angle, in_hist_y_rotated)

# 绘制旋转后的直方图

histSize = math.ceil((max(in_hist_y_rotated)-min(in_hist_y_rotated))/dis)
out_hist_y_rotated = ((c_int) * histSize)()
cdll.CalcHist2(in_hist_x_rotated, in_hist_y_rotated, valueSize, out_hist_y_rotated, histSize, 0)



