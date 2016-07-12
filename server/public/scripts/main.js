var logData = [
];

var LogLine = React.createClass({
    render: function() {
        var obj = this.props.line;
        return (
            <tr> 
                {
                    this.props.fields.map(function(key) {
                        return <td key={key}> {obj[key]} </td>
                })}
            </tr>            
        );    
    }  
});

var LogList = React.createClass({
    componentDidMount: function() {
        setInterval(function() {
            var oldState = this.state;
            oldState.nextId++;
            logData.push({
                id: oldState.nextId,
                tag: "testTag" + oldState.nextId,
                msg: "test message " + oldState.nextId
            });
            this.setState(oldState);
        }.bind(this), 1000);
    },
    getInitialState: function() {
         return {   
            fields: ["id", "tag", "msg"],
            nextId: 0
        };
    },
    render: function() {
        var fieldsCopy = this.state.fields;
        return (    
           <table>
           <thead><tr>
           {this.state.fields.map(function(field) {
                    return <th key={field}> {field} </th>
            })}
           </tr></thead>
           <tbody>
                {this.props.data.map(function(lineData) {
                    return <LogLine key={lineData.id} line={lineData} fields={fieldsCopy} /> 
                })}
           </tbody></table> 
        );    
    }
});

ReactDOM.render(
    <LogList data={logData} />,
    document.getElementById('log')
);
