
import { Component } from '@angular/core';
import { CarObserver, ObserverService } from '../observer.service';
@Component({
    selector: 'lane-detection',
    template: require('./lane-detection.html'),
    styles: [require('./lane-detection.scss')]
})
export class LaneDetectionComponent {
    private observer: CarObserver;

    constructor(private observerService: ObserverService) {}

    ngAfterViewInit() {
        this.observerService.getObserver('lane_detection').subscribe(data => {
            this.observer = data;
            setTimeout(() => 500);
        });
    }


}