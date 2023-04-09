import matplotlib.pyplot as plt

class LineGraph:
    def __init__(self, files):
        self.files = files
        self.data = []

    def read_data(self):
        for file in self.files:
            with open(file, 'r') as f:
                data = f.read().split('\n')
                x = []
                y = []
                z = []
                for row in data:
                    row_data = row.split(',')
                    if len(row_data) == 2:
                        x.append(float(row_data[0]))
                        y.append(float(row_data[1]))
                    elif len(row_data) == 3:
                        x.append(float(row_data[0]))
                        y.append(float(row_data[1]))
                        z.append(float(row_data[2]))
                if len(row_data) == 2:
                  self.data.append((x, y))
                elif len(row_data) == 3:
                  self.data.append((x,y,z))

    def plot_graph(self):
        if len(self.data[1]) == 2:

          for x, y in self.data:
              plt.plot(x, y)

          plt.xlabel('X')
          plt.ylabel('Y')
          plt.title('Line Graph')
          plt.show()
        elif len(self.data[1]) == 3:

          for x, y in self.data:
              plt.plot(x, y)
          
          for x, z in self.data:
              plt.plot(x, z)

          plt.xlabel('X')
          plt.ylabel('Y')
          plt.title('Line Graph')
          plt.legend()
          plt.show()



def main():
    files = ['put.txt', 'get.txt']
    graph = LineGraph(files)
    graph.read_data()
    graph.plot_graph()

if __name__ == '__main__':
    main()