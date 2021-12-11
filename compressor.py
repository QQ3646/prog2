import os.path
from os.path import isfile

finalStr = ""
PATH = os.path.dirname(os.path.realpath(__file__)) + "\\"

includes = []

main_file = "main.cpp"

def recognize(name_of_file, finalStr, includes):
    with open(PATH + name_of_file, "r", encoding="utf-8") as main_file:
        lines_in_file = main_file.readlines()
        for line in lines_in_file:
            if line[0] == '#':
                if line.find('<') == -1:
                    include = line[line.find('"') + 1:line.rfind('"')]
                    if name_of_file.find('/') != -1:
                        include = name_of_file[0:name_of_file.find('/') + 1] + include
                    finalStr += recognize(include, finalStr, includes)
                else:
                    if line not in includes:
                        includes.append(line)
            else:
                if line.find('//') == -1:
                    finalStr += line

    return finalStr



finalStr = recognize(main_file, finalStr, includes)

finalStr = ''.join(includes) + finalStr

with open("fornsu.cpp", "w", encoding='utf-8') as f:
    f.write(finalStr)