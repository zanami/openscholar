(function () {
  var rootPath = Drupal.settings.osRestModulePath,
      restPath = Drupal.settings.restBasePath;

  angular.module('mediaBrowser', [])
  /**
   * Service to maintain the list of files on a user's site
   */
  .factory('FileService', ['$rootScope', '$http', function ($rootScope, $http) {
    var files = [];
    $http.get(restPath+'/file')
      .success(function success(data) {
        files = data.list;
        $rootScope.$broadcast('FileService.changed', files);
      })
      .error(function errorFunc() {
        $http.get({url: restPath}).
          success(success).
          error(errorFunc);
      });

    return {
      getAll: function () {
        return files;
      },
      get: function (fid) {
        for (var i = 0; i < files.length; i++) {
          if (files[i].fid == fid) {
            return files[i];
          }
        }
        throw new Exception('FID not found');
      },
      add: function (file) {
        files.push(file);
        $rootScope.$broadcast('FileService.changed', files);
      },
      edit: function (file) {
        for (var i = 0; i < files.length; i++) {
          if (files[i].fid == file.fid) {
            files[i] = file;
            $rootScope.$broadcast('FileService.changed', files);
            operating = true;
            $http.put
            return true;
          }
        }
        return false;
      },
      delete: function (fid) {
        if (angular.isObject(fid)) {
          fid = fid.fid;
        }
        for (var i = 0; i < files.length; i++) {
          if (files[i].fid == fid) {
            files.splice(i, 1);
            $rootScope.$broadcast('FileService.changed', files);
            return true;
          }
        }
        return false;
      },
      operating: false
    };
  }]).
  controller('BrowserCtrl', ['FileService', '$scope', '$filter', '$http', '$templateCache', function (FileService, $scope, $filter, $http, $templateCache) {
    $scope.files = FileService.getAll();
    $scope.templatePath = rootPath;
    $scope.selection = 0;
    $scope.selection_form = '';

    // Watch for changes in file list
    $scope.$on('FileService.changed', function (event, files) {
      $scope.files = files;
      console.log(files);
    });

    // Filter list of files by a filename fragment
    $scope.queryFilename = function (item) {

      console.log($scope);
      if ($scope.search) {
        if (item.name.indexOf($scope.search) > -1) {
          return true;
        }
        else if (item.orig && item.orig.indexOf($scope.search) > -1) {
          return true;
        }
        return false;
      }
      return true;
    };

    // pager methods
    var currentPage = 0;
    $scope.currentPage = function () {
      return Math.min(currentPage, $scope.numberOfPages()-1);
    };
    $scope.setCurrentPage = function (p) {
      currentPage = p;
    };
    $scope.pageSize = 10;
    $scope.numberOfPages = function () {
      return Math.ceil($filter('filter')($scope.files, $scope.queryFilename).length/$scope.pageSize);
    };

    // selected file
    $scope.setSelection = function (fid) {
      $scope.selection = fid;
      $scope.selected_file = angular.copy(FileService.get(fid));
      $scope.selection_form = rootPath+'/templates/'+$scope.selected_file.form+'.html';
    };

    // preload file edit templates
    $http.get(rootPath+'/templates/file_edit_default.html', {cache:$templateCache});
    $http.get(rootPath+'/templates/file_edit_image.html', {cache:$templateCache});

    // file edit form methods
    $scope.save = function() {
      FileService.edit(selected_file);
    };

    $scope.delete = function() {
      FileService.delete(selected_file);
    };
  }]).
  filter('start', function () {
    return function (input, start) {
      if (input) {
        start = +start;
        return input.slice(start);
      }
      return '';
    }
  });
})();