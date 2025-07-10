import numpy as np
import csv
import os

def generate_test_case(n, std_scale, output_dir="practika_test_data"):
    os.makedirs(output_dir, exist_ok=True)

    freq_min, freq_max = 2000000, 20000000
    length_min, length_max = 100, 10000
    angle_min, angle_max = -90, 90

    # Определяем LSB
    lsb_freq = 1000  # 1 кГц
    lsb_len = 100     # 100 нс
    lsb_ang = 1      # 1 градус
    
    # Средние значения
    mean_freq = (freq_min + freq_max) / 2
    mean_len = (length_min + length_max) / 2
    mean_ang = (angle_min + angle_max) / 2

    # Стандартные отклонения
    std_freq = std_scale * lsb_freq
    std_len = std_scale * lsb_len
    std_ang = std_scale * lsb_ang
    
    # Генерация данных
    freq = np.random.normal(mean_freq, std_freq, n).astype(np.uint32)
    length = np.random.normal(mean_len, std_len, n).astype(np.uint16)
    angle = np.random.normal(mean_ang, std_ang, n).astype(np.int8)
    
    # Расчет эталонных значений
    ref = {
        'count': n,
        'mean_freq': np.mean(freq),
        'var_freq': np.var(freq, ddof=1),
        'mean_len': np.mean(length),
        'var_len': np.var(length, ddof=1),
        'mean_ang': np.mean(angle),
        'var_ang': np.var(angle, ddof=1)
    }
    
    # Сохранение данных
    filename = f"{output_dir}/test_{n}_{std_scale}.csv"
    with open(filename, 'w') as f:
        writer = csv.writer(f)
        writer.writerow(['frequency', 'length', 'angle'])
        for i in range(n):
            writer.writerow([freq[i], length[i], angle[i]])
    
    # Сохранение эталонных значений
    answer_filename = f"{output_dir}/test_{n}_{std_scale}.txt"
    with open(answer_filename, 'w') as f:
        for key, value in ref.items():
            f.write(f"{key}={value}\n")

    return filename, answer_filename

# Генерация всех тестовых случаев
test_cases = [
    (100, 0.1), (100, 1), (100, 10),
    (1000, 0.1), (1000, 1), (1000, 10),
    (10000, 0.1), (10000, 1), (10000, 10)
]

for elements in test_cases:
    data_file, answer_file = generate_test_case(*elements)
    print(f"Создано: {data_file} с {answer_file}")