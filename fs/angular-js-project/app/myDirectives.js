
(function () {
  let myDirectivesModule = angular.module('myDirectives', []);
 
  myDirectivesModule.directive('myBind', function () {
    return {
      link(scope, iElement, iAttrs) {
        iElement.text(scope[iAttrs['myBind']]);

        scope.$watch(iAttrs['myBind'], (newValue) => iElement.text(newValue));
      }
    }
  })

  myDirectivesModule.directive('elementToScope', function () {
    return {
      restrict: 'A',
      compile() {
        return {
          pre(scope, iElement, iAttrs) {
            scope[iAttrs['elementToScope']] = iElement;
          }
        }
      }
    }
  });

  myDirectivesModule.directive('twoWayBind', () => {
    return {
      link(scope, element, attrs) {
        element.on('input', { value: element.val() }, (event) => {

          scope.$apply(() => {
            scope[attrs['twoWayBind']] = event.target.value;
          });
        });

      },
      controller($scope, $element, $attrs) {
        $scope.$watch($attrs['twoWayBind'], (newValue) => $element.val(newValue));
      }
    }
  });

  myDirectivesModule.directive('myOnClick', () => {
    return {
      scope: {
        onClick: "&myOnClick"
      },
      link(scope, element) {
        element.on('click', () => {
          scope.$apply(() => scope.$eval(scope.onClick()))
        });
      }
    }
  });
}());