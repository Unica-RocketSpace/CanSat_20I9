import pyqtgraph as pg


class Graph(pg.PlotItem):
    def __init__(self, parent=None, layout=None, count_plot_lines=None, name=None):
        super().__init__(title=name)
        layout.addItem(self)
        self.plot_lines = []

        self.count_plot_lines = count_plot_lines
        if self.count_plot_lines: self.plot_lines = [self.plot() for i in range(self.count_plot_lines)]


class GraphLayout(pg.GraphicsLayoutWidget):
    def __init__(self, parent=None, centralwidget=None, layout=None, count_of_graphs=0, graph_name=[], count_plot_lines=None):
        super().__init__(centralwidget)
        layout.addWidget(self)
        self.count_of_graphs = count_of_graphs
        self.graphs = []
        i = 0
        if self.count_of_graphs: self.graphs = [Graph(parent=parent, layout=self, count_plot_lines=count_plot_lines,
                                                      name=graph_name[i] if graph_name else '') for i in range(self.count_of_graphs)]

