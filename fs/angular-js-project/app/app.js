'use strict';

//Script content is in an IIFE to isolate it from the global scope.
(function() {
  let module = angular.module('app', []);

  let dogs = [
    { name: "Charlie", description: "Charlie is a very affectionate, energetic 1 year old, female, she needs structure and training. Charlie will become protective of home if she doesn’t understand that you have control. Charlie has done well with cats but does try to play, we recommend they are never be left unsupervised. She is good with dogs but doesn’t meet well she is very in their face which may not please some dogs.", image: "http://dailypicksandflicks.com/wp-content/uploads/2011/08/Smiling-dog-photobomb.jpg" },
    { name: "Bella", description: "A rose by any other name...Meet Anna Bella Rose, also known as Bella. Not one of us is immune to tragedy. Bella’s family endured recent tragedy and this event devastated the family in almost every way it could. They are doing their best and for Bella, the best thing was to get her to a place where she would be safe and able to live the Princess life she had previously become accustomed to. We were honoured to be able to assist and make sure that not only will she be pampered, she will get the medical help that she needs. Welcome to the Misfit family Bella.", image: "https://cdn.images.express.co.uk/img/dynamic/1/590x/dog-650299.jpg" },
    { name: "Freckles", description: "Freckles is an approximately 2-3 year old spayed female looking for a home **in Newfoundland!** She would do best in a home without any other animals, but would like to have dog friends that she can have play dates with. She is working on her leash manners on walks and gets a little bit over-excited when she sees people or other dogs on walks and can be quite rude/amped up. Freckles is crate trained and working on perfecting her housebreaking.", image: "https://www.surfcoast.vic.gov.au/files/assets/public/corporate-images/landing-page-images/doglookingup-800x531.jpg?dimension=pageimage&w=480" },
    { name: "Benny", description: "Charlie is a very affectionate, energetic 1 year old, female, she needs structure and training. Charlie will become protective of home if she doesn’t understand that you have control. Charlie has done well with cats but does try to play, we recommend they are never be left unsupervised. She is good with dogs but doesn’t meet well she is very in their face which may not please some dogs.", image: "https://i.ytimg.com/vi/bDjGJzBdAwY/hqdefault.jpg" },
    { name: "Dakota", description: "A rose by any other name...Meet Anna Bella Rose, also known as Bella. Not one of us is immune to tragedy. Bella’s family endured recent tragedy and this event devastated the family in almost every way it could. They are doing their best and for Bella, the best thing was to get her to a place where she would be safe and able to live the Princess life she had previously become accustomed to. We were honoured to be able to assist and make sure that not only will she be pampered, she will get the medical help that she needs. Welcome to the Misfit family Bella.", image: "https://cdn.images.express.co.uk/img/dynamic/1/590x/dog-650299.jpg", image: "https://i.ytimg.com/vi/r0PFITJ6zE4/hqdefault.jpg" },
    { name: "Alaska", description: "reckles is an approximately 2-3 year old spayed female looking for a home **in Newfoundland!** She would do best in a home without any other animals, but would like to have dog friends that she can have play dates with. She is working on her leash manners on walks and gets a little bit over-excited when she sees people or other dogs on walks and can be quite rude/amped up. Freckles is crate trained and working on perfecting her housebreaking. ", image: "https://i.ytimg.com/vi/JiUbp8sdwvA/hqdefault.jpg" },
  ];

  module.value('dogs', dogs)

  module.controller('DogsController', DogsController);
  
  function DogsController($scope, dogs) {

    $scope.dogs = dogs;
    $scope.dogStyle = "dog";
    $scope.nameStyle = "dog_name";
    $scope.textStyle = "dog_text"
    $scope.photoStyle = "dog_photo";
  }
})();


