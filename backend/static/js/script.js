$(document).ready(function() {
    function fetchWaterLevel() {
        $.get('/get_latest', function(data) {
            var waterLevel = parseInt(data.level);
            var maxHeight = 100; // Max height of the bar in pixels
            var heightPercentage = (waterLevel / maxHeight) * 100;

            $('#waterLevelBar').css('height', heightPercentage + '%');
            $('#waterLevel').text('Water Level: ' + (100 - waterLevel) + ' %');
        });
    }

    setInterval(fetchWaterLevel, 60000);
    fetchWaterLevel();
});
