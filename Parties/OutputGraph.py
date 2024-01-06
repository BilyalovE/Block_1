import numpy as np
import pandas as pd
from matplotlib.animation import FuncAnimation
import matplotlib.pyplot as plt

rawData = pd.read_csv("C:\\Users\\eldar\\source\\method_characteristics\\Parties\\Output.csv", encoding='windows-1251')

parametersNames = rawData.columns.tolist()[2:]
[timeLabel, coordLabel] = rawData.columns.tolist()[:2]
plotsCount = len(parametersNames)
fig = plt.figure()
axes = [plt.subplot(plotsCount, 1, _ + 1) for _ in range(plotsCount)]
data_skip = len(set(rawData[coordLabel]))

def init_func():
    for i in range(len(axes)):
        axes[i].clear()
        axes[i].grid(visible=True)
        axes[i].set_xlabel(coordLabel)
        axes[i].set_ylabel(parametersNames[i])
        coordData = rawData[coordLabel]
        paramData = rawData[parametersNames[i]]
        xLim = [min(coordData) - 0.1 * (max(coordData) - min(coordData)), max(coordData) + 0.1 * (max(coordData) - min(coordData))]
        yLim = [min(paramData) - 0.1 * (max(paramData) - min(paramData)), max(paramData) + 0.1 * (max(paramData) - min(paramData))]
        axes[i].set_xlim(xLim)
        axes[i].set_ylim(yLim)

# Добавленная закрывающая скобка
