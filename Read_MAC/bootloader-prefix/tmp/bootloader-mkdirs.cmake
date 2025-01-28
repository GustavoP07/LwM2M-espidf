# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/gpisso/esp/v5.3.2/esp-idf/components/bootloader/subproject"
  "/home/gpisso/Trabajo_de_grado/Repositorio_Grupo/LwM2M-espidf/Read_MAC/bootloader"
  "/home/gpisso/Trabajo_de_grado/Repositorio_Grupo/LwM2M-espidf/Read_MAC/bootloader-prefix"
  "/home/gpisso/Trabajo_de_grado/Repositorio_Grupo/LwM2M-espidf/Read_MAC/bootloader-prefix/tmp"
  "/home/gpisso/Trabajo_de_grado/Repositorio_Grupo/LwM2M-espidf/Read_MAC/bootloader-prefix/src/bootloader-stamp"
  "/home/gpisso/Trabajo_de_grado/Repositorio_Grupo/LwM2M-espidf/Read_MAC/bootloader-prefix/src"
  "/home/gpisso/Trabajo_de_grado/Repositorio_Grupo/LwM2M-espidf/Read_MAC/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/gpisso/Trabajo_de_grado/Repositorio_Grupo/LwM2M-espidf/Read_MAC/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/gpisso/Trabajo_de_grado/Repositorio_Grupo/LwM2M-espidf/Read_MAC/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
