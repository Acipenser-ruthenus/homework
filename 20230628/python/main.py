import pandas as pd
import matplotlib.pyplot as plt

# 读取数据
data0 = pd.read_csv('C:/Users/U\Desktop/1(1).csv')
# print(data0.head())

# 散点类型统计
print(data0.type.value_counts(ascending=False))

# 绘制散点图
fig, ax = plt.subplots()
scatter = ax.scatter(data0.x, # x轴数据
         data0.y, # y轴数据
         c=data0.type,
         s=6
         )
legend1 = ax.legend(*scatter.legend_elements(), title="Classes")
ax.add_artist(legend1)
plt.show()