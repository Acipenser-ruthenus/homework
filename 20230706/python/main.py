import tkinter as tk
from tkinter import ttk
from tkinter import filedialog
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2Tk
import cv2
import numpy as np
import os
from ctypes import *
import math
from PIL import Image, ImageTk
from matplotlib.figure import Figure

# 加载dll
cdll = CDLL("D:/homework/20230705/CTest2/Project1/x64/Release/setDLL.dll", winmode=0)
WinDLL("D:/homework/20230705/CTest2/Project1/x64/Release/setDLL.dll", winmode=0)


def select_file():
    global selected_file_path
    global data0
    global C_x_values
    global C_y_values
    global C_type_values
    global valueSize
    selected_file_path = filedialog.askopenfilename()
    data0 = pd.read_csv(selected_file_path)
    C_type_values = data0['type'].astype(int).tolist()
    C_x_values = data0['x'].astype(int).tolist()
    C_y_values = data0['y'].astype(int).tolist()
    valueSize = len(C_type_values)
    C_x_values = (c_int * len(C_x_values))(*C_x_values)
    C_y_values = (c_int * len(C_y_values))(*C_y_values)
    C_type_values = (c_int * len(C_type_values))(*C_type_values)
    # 创建绘图容器 散点图
    scatter_ax = fig1.add_subplot(111)
    plot_scatter(scatter_ax)
    canvas1.draw()

def start_hist():
    # 直方图
    # 获取fig的所有子图列表
    Hist0_ax = fig2.add_subplot(111)
    plot_Hist0(Hist0_ax)
    canvas2.draw()

def plot_scatter(scatter_ax):
    scatter_ax.clear()
    scatter_ax.scatter(data0['x'], data0['y'],c=data0.type,s=4,cmap='tab20')
    scatter_ax.set_xlabel('x')
    scatter_ax.set_ylabel('y')
    scatter_ax.set_title('Scatter Plot')
    canvas1.draw()

def plot_Hist0(Hist0_ax):
    global histSize
    global C_out_hist_y
    global C_out_hist_x
    axis_combo_v = axis_combo.get()
    if axis_combo_v == 'y':
        histSize = math.ceil(max(C_y_values) - min(C_y_values))
        C_out_hist_y = ((c_int) * histSize)()
        cdll.CalcHist(C_x_values, C_y_values, C_out_hist_y, valueSize, histSize, 0)
        Hist0_ax.clear()
        Hist0_ax.plot(C_out_hist_y)  # 绘制直方图
        Hist0_ax.set_title('Hist0 Plot')
        canvas2.draw()

    else:
        histSize = math.ceil(max(C_x_values) - min(C_x_values))
        C_out_hist_x = ((c_int) * histSize)()
        cdll.CalcHist(C_x_values, C_y_values, C_out_hist_x, valueSize, histSize, 1)
        Hist0_ax.clear()
        Hist0_ax.plot(C_out_hist_x)  # 绘制直方图
        Hist0_ax.set_title('Hist0 Plot')
        canvas2.draw()

def plot_Hist1(Hist1_ax):
    Hist1_ax.clear()
    Hist1_ax.plot(C_out_hist)  # 绘制直方图
    Hist1_ax.set_title('Hist1 Plot')
    canvas3.draw()

def plot_Hist1_Blur(Hist1_Blur_ax):
    Hist1_Blur_ax.clear()
    Hist1_Blur_ax.plot(C_out_hist1)  # 绘制直方图
    Hist1_Blur_ax.set_title('Hist1_Blur Plot')
    canvas4.draw()
def start_rotating():
    global dis
    angle = float(angle_entry.get())
    dis = max(abs(math.sin(math.radians(angle))), abs(math.cos(math.radians(angle))))
    angle = c_double(angle)
    in_hist_x_rotated = ((c_double) * valueSize)()
    in_hist_y_rotated = ((c_double) * valueSize)()
    cdll.rotatePointsX(C_x_values, C_y_values, valueSize, angle, in_hist_x_rotated)
    cdll.rotatePointsY(C_x_values, C_y_values, valueSize, angle, in_hist_y_rotated)
    axis_combo_v = axis_combo.get()
    global C_out_hist
    global histSize
    if axis_combo_v == 'y':
        histSize = math.ceil((max(in_hist_y_rotated) - min(in_hist_y_rotated)) / dis / 2)
        C_out_hist = ((c_int) * histSize)()
        cdll.CalcHist2(in_hist_x_rotated, in_hist_y_rotated, valueSize, C_out_hist, histSize, 0)
    else:
        histSize = math.ceil((max(in_hist_x_rotated) - min(in_hist_x_rotated)) / dis / 2)
        C_out_hist = ((c_int) * histSize)()
        cdll.CalcHist2(in_hist_x_rotated, in_hist_y_rotated, valueSize, C_out_hist, histSize, 1)

    Hist1_ax = fig3.add_subplot(111)
    plot_Hist1(Hist1_ax)
    canvas3.draw()

# def rotate():

def start_smoothing():
    #这里需要修改 不能分别x和y
    global C_out_hist1
    global C_out_hist
    # angle = c_double(angle)
    window_size = int(smooth_window_entry.get())
    smoothing_algorithm = smooth_algorithm_combo.get()

    C_out_hist1 = ((c_int) * histSize)()
    if smoothing_algorithm == "MeanBlur":
        cdll.MeanBlur(C_out_hist, C_out_hist1, histSize, window_size)
    elif smoothing_algorithm == "MedianBlur":
        cdll.MedianBlur(C_out_hist, C_out_hist1, histSize, window_size)

    C_out_hist = C_out_hist1
    Hist1_Blur_ax = fig4.add_subplot(111)
    plot_Hist1_Blur(Hist1_Blur_ax)
    canvas4.draw()

# 创建主窗口
window = tk.Tk()
window.title("绘图窗口")

# 创建按钮
button_frame = tk.Frame(window)
button_frame.pack(side=tk.TOP, padx=10, pady=10)

# 选择文件按钮
select_button = tk.Button(button_frame, text="选择文件", command=select_file)
select_button.pack(side=tk.LEFT, padx=10, pady=10)

# 选择x轴或者y轴
axis_label = tk.Label(button_frame, text="轴:")
axis_label.pack(side=tk.LEFT, padx=10, pady=10)
axis_combo = ttk.Combobox(button_frame, values=["x", "y"])
axis_combo.pack(side=tk.LEFT, padx=10, pady=10)
axis_combo.current(0)
# 开始绘制直方图按钮
hist_button = tk.Button(button_frame, text="直方图", command=start_hist)
hist_button.pack(side=tk.LEFT, padx=10, pady=10)

# 输入旋转角度
angle_label = tk.Label(button_frame, text="旋转角度:")
angle_label.pack(side=tk.LEFT, padx=10, pady=10)
angle_entry = tk.Entry(button_frame)
angle_entry.pack(side=tk.LEFT, padx=10, pady=10)
# 开始旋转按钮
rotate_button = tk.Button(button_frame, text="开始旋转", command=start_rotating)
rotate_button.pack(side=tk.LEFT, padx=10, pady=10)

# 输入平滑步长
smooth_window_label = tk.Label(button_frame, text="平滑步长:")
smooth_window_label.pack(side=tk.LEFT, padx=10, pady=10)
smooth_window_entry = tk.Entry(button_frame)
smooth_window_entry.pack(side=tk.LEFT, padx=10, pady=10)
smooth_window_entry.insert(tk.END, "3")

# 选择平滑算法
smooth_algorithm_label = tk.Label(button_frame, text="平滑算法:")
smooth_algorithm_label.pack(side=tk.LEFT, padx=10, pady=10)
smooth_algorithm_combo = ttk.Combobox(button_frame, values=["MeanBlur", "MedianBlur"])
smooth_algorithm_combo.pack(side=tk.LEFT, padx=10, pady=10)
smooth_algorithm_combo.current(0)

# 开始平滑按钮
smooth_button = tk.Button(button_frame, text="开始平滑", command=start_smoothing)
smooth_button.pack(side=tk.LEFT, padx=10, pady=10)

# 创建绘图容器1
fig1 = Figure(figsize=(5, 4))
canvas1 = FigureCanvasTkAgg(fig1, master=window)
canvas1.draw()
canvas1.get_tk_widget().pack(side=tk.TOP, padx=10, pady=10)

# 创建绘图容器2
fig2 = Figure(figsize=(5, 4))
canvas2 = FigureCanvasTkAgg(fig2, master=window)
canvas2.get_tk_widget().pack(side=tk.LEFT, padx=10, pady=10)

# 创建绘图容器3
fig3 = Figure(figsize=(5, 4))
canvas3 = FigureCanvasTkAgg(fig3, master=window)
canvas3.get_tk_widget().pack(side=tk.LEFT, padx=10, pady=10)

# 创建绘图容器4
fig4 = Figure(figsize=(5, 4))
canvas4 = FigureCanvasTkAgg(fig4, master=window)
canvas4.get_tk_widget().pack(side=tk.LEFT, padx=10, pady=10)

# 运行主循环
window.mainloop()
