
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
    };
  }
  
  function StringFilter() {
    return {
      'controlType': 'StringFilter',
      'options': {
        'matchType': 'any',
      },
    };
  }
  
  function CategoryFilter() {
    return {
      'controlType': 'CategoryFilter',
      'options': {
        'filterColumnLabel': 'Metric',
        'ui': {
          'allowTyping': false,
          'allowMultiple': true,
          'selectedValuesLayout': 'belowStacked'
        }
      },
      // Define an initial state, i.e. a set of metrics to be initially selected.
      'state': {}
    };
  }
  
  //and add the filters to controllers
  
  //host doesn't quite filter correctly because the link is being matched.  will probalby have to set up an event to load the url instead
  var host = StringFilter();
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
  
  var version = CategoryFilter();
  version.containerId = 'control-version';
  version.options.filterColumnLabel = 'OS Version';
  var versionPicker = new google.visualization.ControlWrapper(version);

  //control-last_visit (would this be better served as a count than a date?
  
  // Create the dashboard.
  //new google.visualization.Dashboard(document.getElementById('dashboard')).bind(agePicker, barChart)).draw(data);   
  new google.visualization.Dashboard(document.getElementById('dashboard')).
  bind([hostPicker, agePicker, vsitesPicker, viewsPicker, versionPicker], [ tableChart]).
  //bind(agePicker, barChart).
  //bind(vsitesPicker, barChart).
  draw(data); 
}


//Set a callback to run when the Google Visualization API is loaded.
google.setOnLoadCallback(drawChart);