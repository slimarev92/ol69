var MVC = (function () {
    'use strict';

    class _View {
        constructor(controller) {
            this.controller = controller;
            this.mainTitle = document.getElementById('main-title');
            this.mainTitle.innerText = controller.getMainTitle();
            this.mainTitle.addEventListener('click', controller);
        }
    }

    function _Model() {
        var _clickCount = 0;

        Object.defineProperty(this, 'mainTitle', {
            get: () => `Click count ${_clickCount++}`
        });
    }

    class _Controller {
        constructor(model) {
            this.model = model;
        }

        getMainTitle() {
            return this.model.mainTitle;
        }

        clickHandler(target) {
            target.innerText = this.model.mainTitle;
        }

        handleEvent(event) {
            event.stopPropagation();

            switch (event.type) {
                case "click":
                    this.clickHandler(event.target)
                    break;
                default:
                    console.log(event.target);
            }
        }
    }

    return {
        Model: _Model,
        View: _View,
        Controller: _Controller
    };
})();

var model = new MVC.Model();
var control = new MVC.Controller(model);
var view = new MVC.View(control);