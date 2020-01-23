'use strict';

//Script content is in an IIFE to isolate it from the global scope.
(function() {
  let module = angular.module('app', []);

  module.provider('CounterService', CounterServiceProvider);
  module.controller('ShowCountController', ['$scope', 'CounterService', ShowCountController]);

  module.config(['CounterServiceProvider', (CounterServiceProvider) => CounterServiceProvider.setStep(5)]);

  module.config(['$provide', function ($provide) {
    $provide.decorator('CounterService', ['$delegate', function ($delegate) {
      $delegate.decrement = () => $delegate.counter -= $delegate.step;

      return $delegate;
    }])
  }]);

  function CounterServiceProvider() {
    var step = 1;

    this.setStep = (value) => step = value;

    this.$get = () => new CounterService(step);
  }

  function CounterService(step) {
    this.counter = 0;
    this.step = step;
    this.increment = () => this.counter += step;
  }

  function ShowCountController($scope, CounterService) {
    $scope.counterService = CounterService; 
  }

})();


