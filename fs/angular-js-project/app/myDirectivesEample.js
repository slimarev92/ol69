(function() {
    let myModule = angular.module('app', ['ng', 'myDirectives']);

    myModule.controller("NameController", ['$scope', '$attrs', function ($scope, $attrs) {
        $scope.name = $attrs['initName'] || 'sasha';
        $scope.func = (message) => {
          $scope.name = message;
        }
      }]);

      myModule.directive('buttonUpdater', () => {
        return {
          templateUrl: "buttonUpdaterTemplate.html",
          controller($scope) {
            $scope.input = "";
          },
          scope: {
            onEvent: "&",
          }
        }
      });
})();