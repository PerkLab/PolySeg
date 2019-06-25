FROM kitware/itk-js-vtk:latest
MAINTAINER PerkLab

WORKDIR /

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
    -DBUILD_SHARED_LIBS:BOOL=OFF \
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} \
    -DCMAKE_INSTALL_PREFIX:PATH=/install-prefix \
    -DBUILD_TESTING:BOOL=OFF \
    -DVTK_DIR:PATH=/VTK-build \
    ../PolySeg && \
  make && \
  make install && \
  cd .. && chmod -R 777 PolySeg-build

RUN ls /install-prefix/lib/PolySeg/libPolySeg.a
RUN ls /install-prefix/lib/PolySeg/
# TODO: For some reason it is installed in this weird directory
# /install-prefix/lib/PolySeg/libPolySeg.a

ARG IMAGE=ohif/polyseg-web
ARG VERSION=latest
ENV DEFAULT_DOCKCROSS_IMAGE ${IMAGE}:${VERSION}
WORKDIR /work