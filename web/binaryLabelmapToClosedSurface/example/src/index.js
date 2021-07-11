import readImageFile from 'itk/readImageFile'
import runPipelineBrowser from 'itk/runPipelineBrowser'
import curry from 'curry'
import IOTypes from 'itk/IOTypes'
import vtkActor from 'vtk.js/Sources/Rendering/Core/Actor';
import vtkFullScreenRenderWindow from 'vtk.js/Sources/Rendering/Misc/FullScreenRenderWindow';
import vtkMapper from 'vtk.js/Sources/Rendering/Core/Mapper';
import vtkPolyData from 'vtk.js/Sources/Common/DataModel/PolyData';

const outputFileInformation = curry(function outputFileInformation(outputTextArea, event) {
outputTextArea.textContent = "Loading...";
const dataTransfer = event.dataTransfer
const files = event.target.files || dataTransfer.files
return readImageFile(null, files[0])
    .then(function({
        image,
        webWorker
    }) {
        webWorker.terminate()
        const args = ['uploadedImage', 'output.vtk.json'];
        const inputs = [{
            path: args[0],
            data: image,
            type: IOTypes.Image,
        }];

        const outputs = [{
            path: args[1],
            type: IOTypes.vtkPolyData
        }];
        return runPipelineBrowser(null, 'hello', args, outputs, inputs)
    }).then(function({
        outputs,
        webWorker
    }) {
        console.warn('DONE!');
        webWorker.terminate()

        console.warn(outputs);

        const polyData = outputs[0].data
        setupVTKScene(polyData);
    });
})

function setupVTKScene(polyData) {
  const fullScreenRenderer = vtkFullScreenRenderWindow.newInstance();
  const renderer = fullScreenRenderer.getRenderer();
  const renderWindow = fullScreenRenderer.getRenderWindow();
  const resetCamera = renderer.resetCamera;
  const render = renderWindow.render;
  const mapper = vtkMapper.newInstance();
  const actor = vtkActor.newInstance();

  actor.setMapper(mapper);

  const poly = vtkPolyData.newInstance(polyData);
  mapper.setInputData(poly);

  renderer.addActor(actor);

  resetCamera();
  render();
}

export {
    outputFileInformation
}