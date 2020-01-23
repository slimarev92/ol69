'use strict';

//Script content is in an IIFE to isolate it from the global scope.
(function() {
  let appModule = angular.module('app', []);

  let numbers = [1000.11, 2000.22, 3000.33, 4000.44, 5000.55, 6000.66];

  appModule.value('Nums', numbers);
  appModule.controller('NumbersController', ['$scope', 'Nums', 'numberFilter', NumbersController]);

  appModule.filter('twice', function doubleFilterProvider() {
    return (value) => value + " " + value;
  });

  appModule.config(['$filterProvider', function($filterProvider) {
    $filterProvider.register('double', function() {
      return (value) => value * 2;
    })
  }])

  function NumbersController($scope, Nums, number) {
    $scope.nums = Nums;
    $scope.filter = number;
  }

  
})();


