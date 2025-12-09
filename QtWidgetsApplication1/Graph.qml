// LineGraph.qml
import QtGraphs
import QtQuick

 
GraphsView {
    
   
    
    Connections{ // NOT NEEDET AT ALL
        target: myModel
        
        function onDataChanged ()
        {
            series.clear()
            console.log("CLICKED!")
            axisX.max = myModel.getMaxX
            console.log("X AFTER CLICK IS " + myModel.getMaxX)
            var data = myModel.getData

            for(var i = 0; i < data.length; i++){
                series.append(data[i]) // Try to change type qList to QVariantList
            }

        }

    }

    anchors.fill: parent
    anchors.margins: 0
    
    theme: GraphsTheme 
    {
        colorScheme: GraphsTheme.ColorScheme.Dark
        seriesColors: ["#2CDE85", "#DBEB00"] 
    }

    axisX: ValueAxis 
    {
        id: axisX
        min: 0
        max: myModel.getMaxX //from Module
        tickInterval: 5
        labelDecimals: -1
        titleText: "nm"
        
    }

    axisY: ValueAxis 
    {
        min: -1
        max: 1 //myModel.getMaxY // from module
        tickInterval: 10
        labelDecimals: -1
        titleText: "Intensity"
    }

    

    //from module 
    SplineSeries 
    {
        
        
        id: series
        //XYPoint { x: 0; y: 0.1 }
        //XYPoint { x: 1; y: 0.5 }
        
    }
    
    
     Component.onCompleted: {
        
        //console.log("COMPLITED!")
        //console.log("MAX AXIS X: " + axisX.max)
        //console.log("CONNECTION: " + myCon)
        //series.clear()
        
        //var data = myModel.qList       
        
        
         //for (var i = 0; i < data.length; i++) 
           // {
	         //   series.append(data[i])
            //}

        //series.append(data)
        
        //series.append(myModel.qList)

    }
 
    //series.remo
  
   

 
    
}