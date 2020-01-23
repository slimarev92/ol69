(function() {
  'use strict'

  let myApp = angular.module('app', ['ngMaterial', 'ngMessages', 'ngRoute']);

  //CONFIG
  myApp.config(['$mdThemingProvider',function ($mdThemingProvider) {
    $mdThemingProvider.theme('default')
      .primaryPalette('blue')
      .accentPalette('blue');
  }]);

  myApp.config(['$locationProvider', function($locationProvider) {
    $locationProvider.html5Mode(true);
    $locationProvider.hashPrefix('');
  }]);

  myApp.config(['$routeProvider', function($routeProvider) {
    $routeProvider.when("/all", {templateUrl: "all_items/template.html"});
    $routeProvider.when("/todo", {templateUrl: "todo_items/template.html"});
    $routeProvider.when("/archive", {templateUrl: "archive_items/template.html"});

    $routeProvider.when("/", {redirectTo: "/all"});
    $routeProvider.otherwise("/all");
  }]);

  //SERVICES
  myApp.factory('TodoItems', ['$window', '$http' ,function($window, $http) {
    let itemsObj = {items: []};

    $http({
      method: 'GET',
      url: 'http://localhost:1678/Service1.svc/GetTodoItems',
    }).then(function successCallback(response) {
        itemsObj.items = response.data;
        console.log("got items from server")
        console.log(response.data);

      }, function errorCallback(response) {
        // called asynchronously if an error occurs
        // or server returns response with an error status.
      });


    itemsObj.removeItem = (item) => {

      console.log("removing:");
      console.log(JSON.stringify([item]));

      $http({
        method: 'POST',
        url: 'http://localhost:1678/Service1.svc/RemoveItems',
        'Content-Type': "text/plain",
        data:  JSON.stringify([item])
      }).then(function successCallback(response) {
          console.log("remove success.");
          console.log(response);
  
        }, function errorCallback(response) {
          console.log("error lol");
          console.log(response);
        });
    };

    itemsObj.addItem = (itemContent) => {

      let newId = 0;

      $http({
        method: 'GET',
        url: 'http://localhost:1678/Service1.svc/RequestId',
      }).then(function successCallback(response) {
          console.log(`got id ${response.data.id} from server.`)
      
          newId = response.data.id;

          itemsObj.items.push({Id: newId, Content: itemContent.trim(), Done: false});
  
          //COULD BE MUCH BETTER
          itemsObj.sync();

        }, function errorCallback(response) {
          console.log(`ERROR: ${response}`)
        });
  
    };

    itemsObj.sync = () => {

      console.log("sending:");
      console.log(JSON.stringify(itemsObj.items));

      $http({
        method: 'POST',
        url: 'http://localhost:1678/Service1.svc/SetTodoItems',
        data:  JSON.stringify(itemsObj.items)
      }).then(function successCallback(response) {
          console.log("sync success.");
          console.log(response);
  
        }, function errorCallback(response) {
          console.log("error lol");
          console.log(response);
        });
    };

    //figure this out
    //$window.onbeforeunload = () => itemsObj.sync();

    return itemsObj;
  }]);

  //CONTROLLERS
  myApp.controller('NavController', ['$scope', '$location', function($scope, $location) {
    $scope.navItem = '';

    $scope.$on("$routeChangeSuccess", () => $scope.navItem = $location.path());
  }]);

  myApp.controller('ItemsController', ['$scope' ,'TodoItems', ($scope,TodoItems) => {
    $scope.getClasses = (item) => {
      var retVal = {};

      retVal['md-list-item-text'] = true;
      retVal['done'] = item.done;

      return retVal;
    };

    $scope.getItems = () => TodoItems.items;

    $scope.removeItem = (item) => {
      let removeIndex = TodoItems.items.indexOf(item);

      TodoItems.items.splice(removeIndex, 1);
      TodoItems.removeItem(item);
    }
  }]);

  myApp.controller('AddItemController', ['$scope', 'TodoItems',
    ($scope, TodoItems) => {
      $scope.sync = TodoItems.sync;
      $scope.currentItemContent = '';

      $scope.addCurrentItem = () => {
          //TodoItems.items.push({Id: -1, Content: $scope.currentItemContent.trim(), Done: false});

          TodoItems.addItem($scope.currentItemContent);

          console.log(TodoItems.items);

          $scope.currentItemContent = '';
          //$scope.sync();
      }
    }]);

})();