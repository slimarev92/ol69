(function() {
    var myModule = angular.module('app', ['ngRoute']);

    myModule.value('contacts', [
        { name: "sasha", phone: "054-680-4222" },
        { name: "tanya", phone: "054-680-5222" },
        { name: "vova", phone: "04-8523405" }
    ]);

    myModule.controller('Searcher', ['$scope', 'contacts', ($scope, contacts) => {
        $scope.currPhone = "";
        $scope.contacts = contacts;
    }]);


    myModule.config(['$routeProvider', '$locationProvider', function ($routeProvider, $locationProvider) {
        $routeProvider
            .when('/', {
                template: '<a href="/about">About</a>'
            })
            .when('/about', {
                template: '<a href="/">Home</a>'
            });

        $locationProvider.html5Mode(true);
    }]);
})();