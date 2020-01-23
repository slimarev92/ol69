'use strict';

//Script content is in an IIFE to isolate it from the global scope.
(function () {
  let appModule = angular.module('app', []);

  appModule.controller('Controller', ['$scope', Controller]);
  appModule.component('sashComponent', {
    template: '<input type="button" value="press here" ng-click="$ctrl.onev({message: $ctrl.name})"> {{$ctrl.name2}}',
    bindings: {
      name: "=",
      onev: "&"
    }

  });

  function Controller($scope) {
    $scope.name = 'sasha';
    $scope.onEvent = (message) => console.log(message);
  }



})();


