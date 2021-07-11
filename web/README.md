### Build the Docker image for cross-compilation

The image is based on `kitware/itk-js-vtk`

```bash
# Docker needs to be running...
docker build -t ohif/polyseg-web .
```

### Run the build

```bash
cd helloPolySeg
npm install
npx itk-js build --image ohif/polyseg-web:latest .
```

#### TODO:
- [ ] Weird that I have to clone the src into /src under helloPolySegsub