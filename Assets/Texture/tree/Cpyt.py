from PIL import Image
import os

def convert_tga_to_png():
    """
    将当前目录下的所有 .tga 文件转换为 .png 文件，
    并保存到当前目录下的 outPng 文件夹中。
    """
    # 获取当前工作目录
    current_directory = os.getcwd()
    
    # 定义输出文件夹路径
    output_folder = os.path.join(current_directory, "outPng")
    
    # 确保输出文件夹存在
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)
        print(f"Created output folder: {output_folder}")

    # 遍历当前目录下的所有文件
    for filename in os.listdir(current_directory):
        # 检查文件是否为 .tga 格式（不区分大小写）
        if filename.lower().endswith(".tga"):
            # 构造完整的输入文件路径
            tga_path = os.path.join(current_directory, filename)
            
            try:
                # 打开 .tga 文件
                with Image.open(tga_path) as img:
                    # 构造输出文件路径（保持文件名不变，仅更改扩展名）
                    png_filename = os.path.splitext(filename)[0] + ".png"
                    png_path = os.path.join(output_folder, png_filename)
                    
                    # 保存为 .png 文件
                    img.save(png_path, "PNG")
                    print(f"Converted: {tga_path} -> {png_path}")
            except Exception as e:
                print(f"Failed to convert {tga_path}: {e}")

if __name__ == "__main__":
    # 调用转换函数
    convert_tga_to_png()