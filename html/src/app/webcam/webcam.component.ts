import { Component } from '@angular/core';

@Component({
    selector: 'webcam',
    template: require('./webcam.html'),
    styles: [require('./webcam.scss')]
})
export class WebcamComponent {
    private interval = 10;
    private updateRoi(event) {

    }

}