def html_to_header(input_html_file, output_h_file):
    with open(input_html_file, 'r', encoding='utf-8') as html_file:
        html_content = html_file.read()

    with open(output_h_file, 'w', encoding='utf-8') as h_file:
        h_file.write("#ifndef WEBPAGE_H\n")
        h_file.write("#define WEBPAGE_H\n\n")
        h_file.write('const char* webpage = R"=====(\n\n')
        h_file.write(html_content)
        h_file.write('\n\n)=====";\n\n')
        h_file.write("#endif\n")

html_to_header('Website-workshop\index.html', 'src\Website\index.h')
