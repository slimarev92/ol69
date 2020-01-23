(function () {
    'use strict';

    function Range(from, to) {
        Object.defineProperty(this, 'from', {
            value: from,
            writable: false
        });

        Object.defineProperty(this, 'to', {
            value: to,
            writable: false
        });
    }

    Range.prototype[Symbol.iterator] = function () {
        let from = this.from;
        const to = this.to;

        const iterator = {
            next() {
                return {
                    done:  from >= to,
                    value:  from >= to ? undefined : from++
                }

            },

            [Symbol.iterator]() { return this; }
        };

        return iterator;
    }

    var myRange = new Range(0, 10);

    for (let i of myRange[Symbol.iterator]()) {
        console.log(i);
    }

})();