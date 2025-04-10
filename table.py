import matplotlib.pyplot as plt

# Input data
buffer_sizes = [1, 32, 256, 1024, 8192, 65536]
write_times = [0.552213, 0.025421, 0.013015, 0.011366, 0.010964, 0.014119]
read_times = [0.635891, 0.117388, 0.095985, 0.092433, 0.093006, 0.091995]

# Calculate throughputs (bytes/s)
total_bytes = 819200  # From BYTES_TO_READ_WRITE definition
write_throughput = [total_bytes / t for t in write_times]
read_throughput = [total_bytes / t for t in read_times]

# Create table data
columns = ('Buffer Size (bytes)', 
          'Write Time (s)', 
          'Write Throughput (bytes/s)', 
          'Read Time (s)', 
          'Read Throughput (bytes/s)')

rows = []
for i in range(len(buffer_sizes)):
    rows.append([
        f"{buffer_sizes[i]:,}",
        f"{write_times[i]:.6f}",
        f"{write_throughput[i]:,.2f}",
        f"{read_times[i]:.6f}",
        f"{read_throughput[i]:,.2f}"
    ])

# Create figure
fig = plt.figure(figsize=(12, 8))
ax = fig.add_subplot(111, frame_on=False)
ax.axis('off')

# Create table
table = plt.table(cellText=rows,
                  colLabels=columns,
                  loc='center',
                  cellLoc='center',
                  colColours=['#f3f3f3']*5)

# Style table
table.auto_set_font_size(False)
table.set_fontsize(10)
table.scale(1.2, 1.5)  # Adjust table scale

# Set title
plt.title("I/O Performance Metrics by Buffer Size", 
         fontsize=14, 
         pad=20,
         weight='bold')

# Save as image
plt.savefig('io_performance_table.png', 
           dpi=300, 
           bbox_inches='tight')
print("Table saved as io_performance_table.png")