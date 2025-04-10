import matplotlib.pyplot as plt
import numpy as np

# Input data
buffer_sizes = [1, 32, 256, 1024, 8192, 65536]
write_times = [0.552213, 0.025421, 0.013015, 0.011366, 0.010964, 0.014119]
read_times = [0.635891, 0.117388, 0.095985, 0.092433, 0.093006, 0.091995]

# Constants
total_bytes = 819200  # 800KB from original code
MB = 1024 * 1024

# Calculate throughputs in MB/s
write_throughput = [(total_bytes/t)/MB for t in write_times]
read_throughput = [(total_bytes/t)/MB for t in read_times]

# Create figure with subplots
plt.figure(figsize=(14, 6))

# Plot formatting parameters
plt.rcParams.update({
    'font.size': 12,
    'axes.titlesize': 14,
    'axes.labelsize': 12,
    'xtick.labelsize': 10,
    'ytick.labelsize': 10
})

# Write Throughput Plot
plt.subplot(1, 2, 1)
plt.semilogx(buffer_sizes, write_throughput, 
            'bo-', linewidth=2, markersize=8,
            label='Write Throughput')
plt.title('Write Throughput vs Buffer Size')
plt.xlabel('Buffer Size (bytes)')
plt.ylabel('Throughput (MB/s)')
plt.grid(True, which='both', linestyle='--', alpha=0.7)
plt.xticks(buffer_sizes, [str(bs) for bs in buffer_sizes])
plt.legend()

# Add peak value annotation
max_write = max(write_throughput)
max_write_idx = write_throughput.index(max_write)
plt.annotate(f'Peak: {max_write:.2f} MB/s\n@ {buffer_sizes[max_write_idx]}B',
             xy=(buffer_sizes[max_write_idx], max_write),
             xytext=(100, max_write*0.8),
             arrowprops=dict(facecolor='black', shrink=0.05))

# Read Throughput Plot
plt.subplot(1, 2, 2)
plt.semilogx(buffer_sizes, read_throughput, 
            'rs--', linewidth=2, markersize=8,
            label='Read Throughput')
plt.title('Read Throughput vs Buffer Size')
plt.xlabel('Buffer Size (bytes)')
plt.ylabel('Throughput (MB/s)')
plt.grid(True, which='both', linestyle='--', alpha=0.7)
plt.xticks(buffer_sizes, [str(bs) for bs in buffer_sizes])
plt.legend()

# Add maximum value annotation
max_read = max(read_throughput)
max_read_idx = read_throughput.index(max_read)
plt.annotate(f'Max: {max_read:.2f} MB/s\n@ {buffer_sizes[max_read_idx]}B',
             xy=(buffer_sizes[max_read_idx], max_read),
             xytext=(1000, max_read*0.8),
             arrowprops=dict(facecolor='black', shrink=0.05))

# Adjust layout and save
plt.tight_layout(pad=3.0)
plt.savefig('throughput_analysis.png', dpi=300, bbox_inches='tight')
plt.close()

print("Plots saved as throughput_analysis.png")