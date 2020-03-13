import React, { useEffect, useState } from 'react';
import './styles.scss';
import AceEditor from 'react-ace';
import "ace-builds/src-noconflict/theme-monokai";
import "ace-builds/src-noconflict/mode-java";
import "ace-builds/src-noconflict/mode-javascript";
import "ace-builds/src-noconflict/mode-python";
import "ace-builds/src-noconflict/mode-ruby";
import "ace-builds/src-noconflict/mode-golang";
import "ace-builds/src-noconflict/mode-csharp";
import {dockerService} from "./dockerService/DockerService";


function App() {
	const [output, setOutput] = useState([]);

	function runCode(code){
		dockerService.getOutputNode(code)
            .then(res => setOutput(res.data))
            .catch(rej => console.log(rej));
	}

  return (
    <div className="App">
      <div className="wrapper">
          <Input title={"Code input"} codeRunner={runCode}>
          </Input>
          <Output title={"Output:"} text={output}/>
      </div>
    </div>
  );
}

function Input({title, codeRunner, children}){
    let code = "";
    const [language, setLanguage] = useState("");

    function runCode(){
        codeRunner(code);
    }

    function onChangeHandler(value){
           code = value;
    }

  return(
    <div>
      {title}
        <AceEditor
            focus={true}
            mode="javascript"
            theme="monokai"
            className="codeInput"
            onChange={value => onChangeHandler(value)}
            showPrintMargin={false}
            enableLiveAutocompletion={true}
            enableBasicAutocompletion={true}
            enableSnippets={true}
            setOptions={{
                enableBasicAutocompletion: false,
                enableLiveAutocompletion: true,
                enableSnippets: true,
                showLineNumbers: true,
                tabSize: 2,
            }}/>
        <FileTypeStatus filetype={"Javascript"}/>
        {children}
        <button className="btn margin-top-20" onClick={runCode}>Run code</button>
    </div>
  )
}

function Output({title, text}){

  return(
    <div className="margin-top-20">
      {title}
      <div  className="console">
          {text.map(output => output)}
      </div>
    </div>
  )
}

function FileTypeStatus({filetype}) {
    return(
        <div className="fileType">
            {filetype}
        </div>
    )
}

export default App;
