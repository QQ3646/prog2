import os.path
from os.path import isfile

finalStr = ""
PATH = os.path.dirname(os.path.realpath(__file__)) + "\\"

# mainf =

with open(PATH + "main.cpp", "r", encoding="utf-8") as main_file:
    lines_in_file = main_file.readlines()

# def recursive_names_search(dir):
#     NAME_OF_CLASSES = []
#     file_list = os.listdir(dir)
#     for i in file_list:
#         if i[0] != '.' and i.lower().find("cmake") == -1 and i.find(".py") == -1 and i != "fornsu.cpp":
#             if isfile(os.path.join(os.path.join(PATH, dir), i)):
#                 temp = find_names_in_class(os.path.join(os.path.join(PATH, dir), i))
#                 if len(temp) > 0:
#                     NAME_OF_CLASSES += temp
#             else:
#                 temp = recursive_names_search(os.path.join(dir, i))
#                 if len(temp) > 0:
#                     NAME_OF_CLASSES += temp
#     return NAME_OF_CLASSES
#
#
# def find_names_in_class(names):
#     temp_names = []
#     with open(names, 'r', encoding="utf-8") as f:
#         end_of_includes = True
#         str_file = f.read()
#         file_lines = str_file.split('\n')
#         for line in file_lines:
#             if line.find("class") != -1 and line.find(";") == -1:
#                 temp_names.append([line[line.find("class ") + len("class "): line.find("{")].strip(), False])
#         it = iter(file_lines)
#         while (line := next(it)) and end_of_includes:
#             if line[0] == '#' and line.find("pragma") == -1:
#                 name = line[len("#include <") if line.find("/") == -1 else line.find("/"):-1]
#                 if name not in dependencies_graph.keys():
#                     dependencies_graph[name] = []
#                 dependencies_graph[name].append(f.name[f.name.rfind('\\') + 1:])
#             else:
#                 end_of_includes = False
#     return temp_names
#
#
#
# dependencies_graph = {}
#
# NAME_OF_CLASSES = recursive_names_search(os.getcwd())
# print("gf")
# for include in dependencies_graph.keys():
#     b = True
#     for name in NAME_OF_CLASSES:
#         if include[:include.find('.') if include.find('.') != -1 else len(include)] == name[0]:
#             b = False
#             break
#     if b:
#         finalStr += f"#include <{include}>\n"
# print("fd")
# file_list = os.listdir()
# for i in file_list:
#     # if i[0] != '.' and i.lower().find("cmake") == -1 and i.find(".py") == -1:
#     if isfile(i):
#         # with open(i, 'r', encoding="utf-8") as f:
#         #     str_file = str(f.read()).split('\n')
#         #     for line in str_file:
#         #         if line.find("class") != -1:
#         #             NAME_OF_CLASSES.append(line[line.find("class ") + len("class "): line.find("{")].strip())
#         NAME_OF_CLASSES.append(find_names_in_class(i))
#     else:
#         temp_fl = os.listdir(i)
#         for files in temp_fl:
#             if i[0] != '.' and i.lower().find("cmake") == -1 and i.find(".py") == -1:
#                 if isfile(i):
#                     with open(i, 'r', encoding="utf-8") as f:
#                         str_file = str(f.read()).split('\n')
#                         for line in str_file:
#                             if line.find("class") != -1:
#                                 NAME_OF_CLASSES.append(
#                                     line[line.find("class ") + len("class "): line.find("{")].strip())
#
# mainf, classf, headerf = open(PATH + "main.cpp", "r", encoding="utf-8"), \
#                          open(PATH + f"{NAME_OF_CLASS}.cpp", "r", encoding="utf-8"), \
#                          open(PATH + f"{NAME_OF_CLASS}.h", "r", encoding="utf-8")
#
#
# includes = []
# currentf = mainf
# newL = currentf.readline()
# for j in range(2):
#     while newL.find("include") != -1:
#         if newL.find(NAME_OF_CLASS) == -1:
#             for i in includes:
#                 if i == newL:
#                     break
#             else:
#                 includes.append(newL)
#         newL = currentf.readline()
#     currentf = classf
#
# finalStr += "".join(includes) + "\n"
#
# newL = headerf.readline()
# while newL.find(NAME_OF_CLASS + "(") == -1:
#     finalStr += newL
#     newL = headerf.readline()
# headerf.close()
#
# newL = classf.readline()
# while newL.find(NAME_OF_CLASS) == -1 or newL.find("include") != -1:
#     newL = classf.readline()
# while newL != "":
#     finalStr += "   " + newL.replace(f"{NAME_OF_CLASS}::", "")
#     newL = classf.readline()
# finalStr += "};\n\n"
#
# newL = mainf.readline()
# while newL != "":
#     finalStr += newL
#     newL = mainf.readline()
#
# with open(PATH + "fornsu.cpp", "w", encoding="utf-8") as f:
#     f.write(finalStr)
#
# mainf.close()
# classf.close()
# ёёёёёёёёёёёёввыфвфывфывфымсмаправпвапвапвапаппавппвапвп