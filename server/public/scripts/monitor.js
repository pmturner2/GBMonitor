var search = location.search.substring(1);

// var Node = React.createClass({       
//     render: function() {
//         var key = 0;
//         if (this.props.w > 0 && this.props.h > 0) {            
//             return (
                
                     
//                 <div>
//                 <rect key={key} x={this.props.x} y={this.props.y} width={this.props.w} height={this.props.h} className="rect" />

//                     {
//                         // Recursively render children.
//                         this.props.children.map(function(child) {                                                        
//                             return (
//                                 <Node key={++key} x={child.bb_x} y={child.bb_y} w={child.bb_w} h={child.bb_h} children={child.children} parentW={this.props.w} parentH={this.props.h} sceneW={this.props.sceneW} sceneH={this.props.sceneH} />                                
//                             );                            
//                         }.bind(this))
//                     }
//                 </div>
//             );
//         } else {
//             return (                
//                     <div>
//                     {
//                     // Recursively render children.
//                         this.props.children.map(function(child) {                            
//                             return (
//                                 <Node key={++key} x={child.bb_x} y={child.bb_y} w={child.bb_w} h={child.bb_h} children={child.children} parentW={this.props.parentW} parentH={this.props.parentH} sceneW={this.props.sceneW} sceneH={this.props.sceneH} />                                
//                             );                            
//                         }.bind(this))
//                     }
//                     </div>
//             );
             
//         }
//     }
// });

var NodeHierarchy = React.createClass({
    loadMonitor: function(uniqueId) {
            var socket = io.connect();
    
            socket.on('message', function(data) {
                alert(data.message);
            });
    
            socket.on('connected', function(data) {        
                console.log("Connected (from react.js file): Port: " + data.port);
        
                socket.emit('join', {
                    room: 'room_' + uniqueId
                });
            });

            socket.on('log', function(data) {
                this.setState(data.message);

                for (var f in data.message) {
                    console.log("f: " + f + " message[f]: ! " + JSON.stringify(data.message[f], null, 2));
                    // renderHierarchy(data.message.scene);
                }
            }.bind(this));
    },
    initSocketIO: function() {
        if (search == undefined || search == null || search == "") {
            // No unique id specification, go to connect screen.
            // loadLogin();
        } else {
            // Specified unique id in the screen, connect.
            // alert("x " + search + "x");  
            this.loadMonitor(search);  
        }        
    },
    componentDidMount: function() {
        this.initSocketIO();
//         // setInterval(function() {
//         //     var oldState = this.state;
//         //     oldState.nextId++;
//         //     logData.push({
//         //         id: oldState.nextId,
//         //         tag: "testTag" + oldState.nextId,
//         //         msg: "test message " + oldState.nextId
//         //     });
//         //     this.setState(oldState);
//         // }.bind(this), 1000);
    },
    getInitialState: function() {
         return {
            scene: {},
            sceneWidth: 0,
            sceneHeight: 0
        };
    },
    // renderNode: function(node, nodeKey) {
    //     // TODO: make modular
    //     if (node.bb_w > 0 && node.bb_h > 0) {

    //     return (
    //         <rect key={nodeKey} x={node.bb_x} y={node.bb_y} width={node.bb_w} height={node.bb_h} className="rect">
    //             {
    //                 // Recursively render children.
    //                 node.children.map(this.renderNode, ++nodeKey)
    //             }
    //         </rect>
    //     );
    //  } else {
    //     return (
    //         <div key={nodeKey}>
    //         {
    //             node.children.map(this.renderNode, ++nodeKey)
    //         }
    //         </div>
    //     );
    //  }
    // },

    addRect: function(node, rects) {
        rects.push({
            x: node.bb_x,
            y: node.bb_y,
            width: node.bb_w,
            height: node.bb_h
        });
    },
    buildRects: function(node, rects) {
        this.addRect(node, rects);
        if (node.children != undefined) {
        node.children.map(function(child) {                                                        
                            this.buildRects(child, rects);
                        }.bind(this));   
        } 
    },
    render: function() {
        var key = 0;
        console.log("NodeHierarchy !! Calling Render!!");
        var rects=[];
        if (this.state.scene != undefined && this.state.scene.child != undefined) {
            this.buildRects(this.state.scene.child, rects);
            return (                
                <svg width={this.state.sceneWidth} height={this.state.sceneHeight}>                    
                        {
                        // Recursively render children.
                        rects.map(function(child) {                                                        
                            return (
                                <rect key={++key} x={child.x} y={ this.state.sceneHeight - child.y - child.height} width={child.width} height={child.height} className="rect" />                                
                            );                            
                        }.bind(this))
                        }      
                </svg>                
            );
        } else {
            return (<div></div>);
        }        
    }
});

ReactDOM.render(
    <NodeHierarchy />,
    document.getElementById('content')
);

    // <svg width={this.state.screenWidth} height={this.state.screenHeight}>
    //             { renderNode(this.state.scene) }
    //         </svg>           
    //     });    