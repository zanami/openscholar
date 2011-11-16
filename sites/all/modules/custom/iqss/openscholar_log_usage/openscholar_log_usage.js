
//Load the Visualization API and the piechart package.
google.load('visualization', '1.1', {'packages':['controls','table']});


function drawChart() {
  var data = google.visualization.arrayToDataTable( Drupal.settings.openscholar_log_usage_data );
  
  var tableChart = new google.visualization.ChartWrapper({
    'chartType': 'Table',
    'containerId': 'table_div',
    'options': {
      'showRowNumber': true,
      'allowHtml': true,
      'sortAscending': false,
      'sortColumn': 3,
      
    },
  });
  /*
  var barChart = new google.visualization.ChartWrapper({
    'chartType': 'BarChart',
    'containerId': 'chart_div',
    'options': {
      'width': 800,
      'height': Drupal.settings.openscholar_log_usage_data.length * 30,
      'chartArea': {top: 0, right: 0, bottom: 0},
     },
    'view': {'columns': [0,1]}, //host, vsite //views = 3, but on different scale
    
  });
        <div id="control-host"></div>
      <div id="last_visit-views"></div>
      <div id="version"></div>
      
  */
  
  //set up some filters so we can add controls with slightly less copy/paste
  function RangeFilter() {
    return {
      'controlType': 'NumberRangeFilter',
      'options': {
        'ui': {
          'labelStacking': 'vertical',
          'allowTyping': false,
          'allowMultiple': false,
        }
      } 
    }
  }
  
  function stringFilter() {
    return {
      'controlType': 'StringFilter',
      'options': {
        'matchType': 'any',
      },
    }
  }
  
  var host = stringFilter();
  host.containerId = 'control-host';
  host.options.filterColumnLabel = 'Host';
  var hostPicker = new google.visualization.ControlWrapper(host);
  
  var age = RangeFilter();
  age.containerId = 'control-age';
  age.options.filterColumnLabel = 'Age';
  age.options.ui = {'label': 'Age (In Weeks)'};
  var agePicker = new google.visualization.ControlWrapper(age);

  var vsites = RangeFilter();
  vsites.containerId = 'control-vsites';
  vsites.options.filterColumnLabel = 'Sites';
  var vsitesPicker = new google.visualization.ControlWrapper(vsites);
  
  var views = RangeFilter();
  views.containerId = 'control-views';
  views.options.filterColumnLabel = 'Views';
  var viewsPicker = new google.visualization.ControlWrapper(views);
  
  // Create the dashboard.
  //new google.visualization.Dashboard(document.getElementById('dashboard')).bind(agePicker, barChart)).draw(data);   
  new google.visualization.Dashboard(document.getElementById('dashboard')).
  bind([hostPicker, agePicker, vsitesPicker, viewsPicker], [ tableChart]).
  //bind(agePicker, barChart).
  //bind(vsitesPicker, barChart).
  draw(data); 
}


//Set a callback to run when the Google Visualization API is loaded.
google.setOnLoadCallback(drawChart);