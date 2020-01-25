(function () {
  'use strict'

  let myApp = angular.module('app', ['ngMaterial', 'ngMessages', 'ui.router']);

  //CONFIG
  myApp.config(['$mdThemingProvider', function ($mdThemingProvider) {
    $mdThemingProvider.theme('default')
      .primaryPalette('blue')
      .accentPalette('blue');
  }]);

  myApp.config(['$stateProvider', ($stateProvider) => {
    let allState = {
      name: "all",
      url: "/all",
      template: '<item-list type="all"></items>'
    }

    let todoState = {
      name: "todo",
      url: "/todo",
      template: '<item-list type="todo"></items>'
    }

    let archiveState = {
      name: "archive",
      url: "/archive",
      template: '<item-list type="archive"></items>'
    }

    $stateProvider.state(allState);
    $stateProvider.state(todoState);
    $stateProvider.state(archiveState);
  }]);


  //SERVICES
  myApp.factory('todoItemsService', ['$window', '$http', function ($window, $http) {
    let itemsObj = { items: [] };

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
        data: JSON.stringify([item])
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

        itemsObj.items.push({ Id: newId, Content: itemContent.trim(), Done: false });

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
        data: JSON.stringify(itemsObj.items)
      }).then(function successCallback(response) {
        console.log("sync success.");
        console.log(response);

      }, function errorCallback(response) {
        console.log("error lol");
        console.log(response);
      });
    };

    return itemsObj;
  }]);


  //CONTROLLERS + COMPONENTS
  myApp.controller('NavController', ['$scope', function ($scope) {
    //fix later!
    $scope.navItem = 'all';
  }]);

  myApp.component('itemList', {
    controller: ['todoItemsService', function (todoItemsService) {
      this.getClasses = (item) => {
        var retVal = {};

        retVal['md-list-item-text'] = true;
        retVal['done'] = item.done;

        return retVal;
      };

      this.todoItems = todoItemsService.items;

      this.removeItem = (itemToRemove) => {
        todoItemsService.items.splice(todoItemsService.items.indexOf(itemToRemove), 1);
        todoItemsService.removeItem(itemToRemove);
      }

      this.sync = todoItemsService.sync;
    }],
    bindings: {
      type: "@"
    },  
    templateUrl: ['$attrs', ($attrs) => { return $attrs.type + "_items" + "/template.html" }
  ]
  });

  myApp.controller('AddItemController', 
    ['$scope',
      'todoItemsService',
      ($scope, todoItemsService) => {
        $scope.sync = todoItemsService.sync;
        $scope.currentItemContent = '';

        $scope.addCurrentItem = () => {
          todoItemsService.addItem($scope.currentItemContent);

          console.log(todoItemsService.items);

          $scope.currentItemContent = '';
        }
      }
    ]);
})();