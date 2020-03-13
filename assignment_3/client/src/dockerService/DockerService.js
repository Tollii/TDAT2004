import Axios from 'axios';


class DockerService{

    getOutputNode(code){
        return Axios.post('http://localhost:9001/run/node', {input : code});
    }

}

export let dockerService = new DockerService();
