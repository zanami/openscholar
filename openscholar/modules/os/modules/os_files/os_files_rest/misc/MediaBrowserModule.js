(function () {
  var rootPath = Drupal.settings.osRestModulePath,
      restPath = Drupal.settings.restBasePath;

  angular.module('mediaBrowser', []).
  /**
   * Service to maintain the list of files on a user's site
   */
    factory('FileService', ['$rootScope', '$http', function ($rootScope, $http) {
      var files = [],
        api = {
        add: function (file) {
          files.push(file);
          $rootScope.$broadcast('FileService.changed', files);
        },
        edit: function (file) {
          for (var i = 0; i < files.length; i++) {
            if (this.files[i].fid == file.fid) {
              this.files[i] = file;
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
            if (this.files[i].fid == fid) {
              this.files.splice(i, 1);
              $rootScope.$broadcast('FileService.changed', files);
              return true;
            }
          }
          return false;
        }
      };

      $http.get({url: restPath}).
        success(function success(data) {
          ret.files = data.list;
          $rootScope.$broadcast('FileService.changed', files);
        }).
        error(function errorFunc() {
          $http.get({url: restPath}).
            success(success).
            error(errorFunc);
        });

      return api;
    }]).
    config(['$routeProvider', function($routeProvider) {
      $routeProvider.
        when('/upload', {
          templateUrl: path+'/templates/upload.html',
          controller: UploadCtrl
        }).
        when('/internet', {
          templateUrl: path+'/templates/internet.html',
          controller: InternetCtrl
        }).
        when('/browser', {
          templateUrl: path+'/templates/browser.html',
          controller: BrowserCtrl
        }).
        when('/edit/:fileId', {
          templateUrl: path+'/templates/fileEdit.html',
          controller: FileEditCtrl
        }).
        otherwise({
          redirectTo: '/browser'
        });
    }]).
  controller('BrowserCtrl', ['FileService', $scope, function (FileService, $scope) {
      $scope.files = FileService.file;

      $scope.$on('FileService.changed', function (event, files) {
        $scope.files = files;
      });
    }]);

})();