const path = require('path')
const runPipelineNode = require('itk/runPipelineNode')
const readImageLocalFile = require('itk/readImageLocalFile')
const IOTypes = require('itk/IOTypes')

const testImageInputFilePath = 'Segmentation-label.nii';

readImageLocalFile(testImageInputFilePath)
    .then(function(image) {
        const pipelinePath = path.resolve(__dirname, 'web-build', 'hello')
        const args = [testImageInputFilePath + '.json', 'output.vtk.json'];
        const inputs = [{
            path: args[0],
            data: image,
            type: IOTypes.Image,
        }];

        const desiredOutputs = [{
            path: args[1],
            type: IOTypes.vtkPolyData
        }];
        return runPipelineNode(pipelinePath, args, desiredOutputs, inputs)
            .then(function({
                stdout,
                stderr,
                outputs
            }) {
                console.warn(outputs[0].data.points)
            })
    })