import struct

def write_to_bin_file(src_path: str, target_path: str):

    with open(src_path, 'r', encoding='utf-8') as src_file, open(target_path, 'wb') as target_file:

        for _ in range(2):
            next(src_file)

        for line in src_file:
            infos = line.split(' ')
            x = float(infos[0])
            y = float(infos[1])
            z = float(infos[2]) - float(infos[3])
            target_file.write(struct.pack('fff', x, y, z))
            
            
        
if __name__ == '__main__':
    
    write_to_bin_file('../resource/data.txt', '../resource/data.bin')