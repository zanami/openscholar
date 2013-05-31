(function () {
  var rootPath = Drupal.settings.osRestModulePath,
      restPath = Drupal.settings.restBasePath;

  angular.module('mediaBrowser', []).
  /**
   * Service to maintain the list of files on a user's site
   */
  factory('FileService', ['$rootScope', '$http', function ($rootScope, $http) {
    var files = [];
    $http.get({url: restPath})
      .success(function success(data) {
        ret.files = data.list;
        $rootScope.$broadcast('FileService.changed', files);
      })
      .error(function errorFunc() {
        $http.get({url: restPath}).
          success(success).
          error(errorFunc);
      });

    return {
      add: function (file) {
        files.push(file);
        $rootScope.$broadcast('FileService.changed', files);
      },
      edit: function (file) {
        for (var i = 0; i < files.length; i++) {
          if (files[i].fid == file.fid) {
            files[i] = file;
            $rootScope.$broadcast('FileService.changed', files);
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
      }
    };
  }]).
  /**
   * Sets up paths for when the
   */
  config(['$routeProvider', function($routeProvider) {
    $routeProvider.
      when('/browser/upload', {
        templateUrl: path+'/templates/upload.html',
        controller: UploadCtrl
      }).
      when('browser/internet', {
        templateUrl: path+'/templates/internet.html',
        controller: InternetCtrl
      }).
      when('/browser/list', {
        templateUrl: path+'/templates/browser.html',
        controller: BrowserCtrl
      }).
      when('/browser/edit/:fileId', {
        templateUrl: path+'/templates/fileEdit.html',
        controller: FileEditCtrl
      }).
      when('/', {}).    // do nothing when the browser isn't open
      otherwise({
        redirectTo: '/'
      });
  }]).
  controller('BrowserCtrl', ['FileService', '$scope', function (FileService, $scope) {
    $scope.files = FileService.file;

    $scope.$on('FileService.changed', function (event, files) {
      $scope.files = files;
    });

    $scope.queryFilename = function (item) {
      if (item.name.indexOf($scope.filename) > -1) {
        return true;
      }
      else if (item.orig.indexOf($scope.filename) > -1) {
        return true;
      }
      return false;
    }
  }]);

})();