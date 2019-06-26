FROM kitware/itk-js-vtk:latest
MAINTAINER PerkLab

WORKDIR /

COPY ITKBridgeJavaScript.cmake /usr/share/cmake-3.13/Modules/

# 2019-01-03 + Emscripten patches
# ENV VTK_GIT_TAG 519acbcfba80c6e3861a2254781371cd4dfc78fb
# RUN git clone https://github.com/thewtex/VTK.git && \
#   cd VTK && \
#   git checkout ${VTK_GIT_TAG} && \
#   cd ../ && \
#   mkdir VTK-build && \
#   cd VTK-build && \
#   cmake \
#     -G Ninja \
#     -DCMAKE_BUILD_TYPE:STRING=Release \
#     -DBUILD_SHARED_LIBS:BOOL=OFF \
#     -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} \
#     -DCMAKE_INSTALL_PREFIX:PATH=/install-prefix \
#     -DBUILD_EXAMPLES:BOOL=OFF \
#     -DBUILD_TESTING:BOOL=OFF \
#     -DVTK_RENDERING_BACKEND:STRING=None \
#     -DModule_vtkIOExport:BOOL=ON \
#     -DVTK_NO_PLATFORM_SOCKETS:BOOL=ON \
#     -DCMAKE_USE_PTHREADS_INIT:BOOL=OFF \
#     ../VTK && \
#   ninja -j7 && \
#   find . -name '*.o' -delete && \
#   cd .. && chmod -R 777 VTK-build

# # 2019-01-27
# ENV ITKVtkGlue_GIT_TAG 6f3cd6fd697ec19ee12b65041537594bf295c0ca
# RUN git clone https://github.com/InsightSoftwareConsortium/ITKVtkGlue.git && \
#   cd ITKVtkGlue && \
#   git checkout ${ITKVtkGlue_GIT_TAG} && \
#   cd ../ && \
#   mkdir ITKVtkGlue-build && \
#   cd ITKVtkGlue-build && \
#   cmake \
#     -G Ninja \
#     -DCMAKE_BUILD_TYPE:STRING=Release \
#     -DBUILD_SHARED_LIBS:BOOL=OFF \
#     -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} \
#     -DCMAKE_INSTALL_PREFIX:PATH=/install-prefix \
#     -DBUILD_TESTING:BOOL=OFF \
#     -DITK_DIR:PATH=/ITK-build \
#     -DVTK_DIR:PATH=/VTK-build \
#     ../ITKVtkGlue && \
#   ninja -j7 && \
#   find . -name '*.o' -delete && \
#   cd .. && chmod -R 777 ITK-build

# 2019-03-11
ENV PolySeg_GIT_TAG 74f1665a5c2d15e22d57e3a44821ad1c15b739aa
RUN git clone https://github.com/PerkLab/PolySeg.git && \
  cd PolySeg && \
  git checkout ${PolySeg_GIT_TAG} && \
  cd ../ && \
  mkdir PolySeg-build && \
  cd PolySeg-build && \
  cmake \
    -DCMAKE_BUILD_TYPE:STRING=Release \
    -DBUILD_SHARED_LIBS:BOOL=ON \
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} \
    -DCMAKE_INSTALL_PREFIX:PATH=/install-prefix \
    -DBUILD_TESTING:BOOL=OFF \
    -DVTK_DIR:PATH=/VTK-build \
    ../PolySeg && \
  make && \
  make install && \
  cd .. && chmod -R 777 PolySeg-build

RUN ls /install-prefix/lib/PolySeg/
# TODO: For some reason it is installed in this weird directory
# /install-prefix/lib/PolySeg/libPolySeg.so

ARG IMAGE=ohif/polyseg-web
ARG VERSION=latest
ENV DEFAULT_DOCKCROSS_IMAGE ${IMAGE}:${VERSION}
WORKDIR /work