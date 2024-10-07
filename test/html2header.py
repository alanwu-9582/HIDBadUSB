def html_to_header(input_html_file, output_h_file):
    # 開啟並讀取 HTML 檔案
    with open(input_html_file, 'r', encoding='utf-8') as html_file:
        html_content = html_file.read()

    # 創建 .h 檔案並寫入內容
    with open(output_h_file, 'w', encoding='utf-8') as h_file:
        h_file.write("#ifndef WEBPAGE_H\n")
        h_file.write("#define WEBPAGE_H\n\n")
        h_file.write('const char* webpage = R"=====(\n\n')
        h_file.write(html_content)
        h_file.write('\n\n)=====";\n\n')
        h_file.write("#endif\n")

# 使用範例：將 'index.html' 轉換為 'webpage.h'
html_to_header('test\index.html', 'test\index.h')
