var search = location.search.substring(1);

if (search == undefined || search == null || search == "") {
    // No unique id specification, go to connect screen.
    // loadLogin();
} else {
    // Specified unique id in the screen, connect.
    loadMonitor(search);  
}

function loadMonitor(uniqueId) {
    var socket = io.connect();
    
    socket.on('connected', function(data) {        
        console.log("Connected. Port: " + data.port);
    
        // Send request to server to assign this client to specified room.    
        socket.emit('join', {
            room: 'room_' + uniqueId
        });
    });

    socket.on('log', function(data) {
       update(data.message); 
       

         //        for (var f in data.message) {
         //            console.log("f: " + f + " message[f]: ! " + JSON.stringify(data.message[f], null, 2));
         //            // renderHierarchy(data.message.scene);
         // }
    });
}

var diagonal = d3.svg.diagonal()
    .projection(function(d) { return [d.y, d.x]; });

var margin = {
    top: 20,
    right: 120,
    bottom: 20,
    left: 120
}

var sceneWidth = 0;
var sceneHeight = 0;
var width = 0;
var height = 0;

function updateBounds(newSceneWidth, newSceneHeight) {
    // if (sceneWidth == newSceneWidth && sceneHeight == newSceneHeight) {
    //     return;
    // }
    sceneWidth = newSceneWidth;
    sceneHeight = newSceneHeight;
    width = sceneWidth - margin.right - margin.left;
    height = sceneHeight - margin.top - margin.bottom;
    buildTree();
}

var tree;
var svg;
function buildTree() {
    // TODO: reset old state?
    tree = d3.layout.tree()
        .size([height, width]);
    svg = d3.select("body").append("svg")
        .attr("width", sceneWidth)
        .attr("height", sceneHeight)
        .append("g")
        .attr("transform", "translate(" + margin.left + "," + margin.top + ")");
}
var i = 0;
function populateNodes(data) {
    console.log("populateNodes: " + JSON.stringify(data));
    // return;

    var nodes = tree.nodes(data).reverse();
    var links = tree.links(nodes);

    console.log("node length: " + nodes.length + " links length: " + links.length);
    // console.log("nodes: " + JSON.stringify(nodes));

    nodes.forEach(function(d) {
        d.y = d.depth * 180;
    });

    var node = svg.selectAll("g.node")
        .data(nodes, function(d) {
            return d.id || (d.id = ++i);
        });

    var nodeEnter = node.enter().append("g")
        .attr("class", "node")
        .attr("transform", function(d) {
            return "translate(" + d.y + "," + d.x +")";
        });

    nodeEnter.append("circle")
        .attr("r", 10)
        .style("fill", "#fff");

    nodeEnter.append("text")
        .attr("x", function(d) {
            return d.children || d._children ? -13 : 13; 
        })
        .attr("dy", ".35em")
        .attr("text-anchor", function(d) {
            return d.children || d._children ? "end" : "start";
        })
        .text(function(d) { return d.name; })
        .style("fill-opacity", 1);

    node.exit().remove();

    var link = svg.selectAll("path.link")
        .data(links, function(d) {
            return d.target.id;
        });
    link.enter().insert("path", "g")
        .attr("class", "link")
        .attr("d", diagonal);
    link.exit().remove();
}

function update(data) {
    // for (var f in data.scene) {
    //     console.log("data.scene["+f+"] = " + data[f]);
    // }
    
    // for (var i = 0; i < data.scene.length; ++i) {
    //     console.log("data.scene["+i+"] = " + data.scene]
    // }
    // return;

    // updateBounds(data.sceneWidth, data.sceneHeight);
    if (data.scene != undefined) {
        populateNodes(data.scene[0]);//fake[0]);//data.scene.child);
    } else {
        console.log("Some undefined in update " + JSON.stringify(data.scene));
    }
    // d3.json("treeData2.json", function(error, treeData) {
    //     var root = treeData[0];
    //     populateNodes(root);
    // });
}

updateBounds(2000, 2000);


var fake=[
  { 
  name: "Top Level",
  parent: "null",
  children: [
    {
      name: "Level 2: A",
      parent: "Top Level",
      children: [
        {
          name: "Son of A",
          parent: "Level 2: A"
        },
        {
          name: "Daughter of A",
          parent: "Level 2: A"
        }
      ]
    },
    {
      name: "Level 2: B",
      parent: "Top Level"
    }
  ]
}
];

// OLD -->

    // addRect: function(node, rects) {
    //     rects.push({
    //         x: node.bb_x,
    //         y: node.bb_y,
    //         width: node.bb_w,
    //         height: node.bb_h
    //     });
    // },
    // buildRects: function(node, rects) {
    //     this.addRect(node, rects);
    //     if (node.children != undefined) {
    //     node.children.map(function(child) {                                                        
    //                         this.buildRects(child, rects);
    //                     }.bind(this));   
    //     } 
    // },
//     render: function() {
//         var key = 0;
//         console.log("NodeHierarchy !! Calling Render!!");
//         var rects=[];
//         if (this.state.scene != undefined && this.state.scene.child != undefined) {
//             // this.buildRects(this.state.scene.child, rects);
//             return (                
//                 <svg width={this.state.sceneWidth} height={this.state.sceneHeight}>                    
//                         {
//                         // Recursively render children.
//                         rects.map(function(child) {                                                        
//                             return (
//                                 <rect key={++key} x={child.x} y={ this.state.sceneHeight - child.y - child.height} width={child.width} height={child.height} className="rect" />                                
//                             );                            
//                         }.bind(this))
//                         }      
//                 </svg>                
//             );
//         } else {
//             return (<div></div>);
//         }        
//     }

// });
