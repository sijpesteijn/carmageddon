import { Component, ViewChild } from '@angular/core';
import { CarObserver, ObserverService, Roi } from '../observer.service';
import { StreamComponent } from '../../webcam/stream/stream.component';

const MAX_WIDTH = 640;
const MAX_HEIGHT = 480;
@Component({
    selector: 'traffic-light',
    template: require('./traffic-light.html'),
    styles: [require('./traffic-light.scss')]
})
export class TrafficLightObserverComponent {
    @ViewChild(StreamComponent)
    private stream: StreamComponent;
    private observer: CarObserver;
    private interval = 1;

    constructor(private observerService: ObserverService) {}

    ngAfterViewInit() {
        this.observerService.getObserver('traffic_light').subscribe(data => {
           this.observer = data;
           setTimeout(() => 500);
        });
    }

    private setRoi() {
        if (this.observer) {
            if (this.observer.roi.x > MAX_WIDTH) {
                this.observer.roi.x = MAX_WIDTH;
            }
            if (this.observer.roi.x + this.observer.roi.width > MAX_WIDTH) {
                this.observer.roi.width = MAX_WIDTH - this.observer.roi.x;
            }
            if (this.observer.roi.y > MAX_HEIGHT) {
                this.observer.roi.y = MAX_HEIGHT;
            }
            if (this.observer.roi.y + this.observer.roi.height > MAX_HEIGHT) {
                this.observer.roi.height = MAX_HEIGHT - this.observer.roi.y;
            }
        }
        this.stream.theRoi = this.observer.roi;
    }

    private updateRoi(event: any) {
        this.observer.roi = event;
        this.observerService.saveObserver('traffic_light', this.observer).subscribe(observer => this.observer = observer);
    }

    private updateObserver() {
        this.observer.active =  (Number(this.observer.active));
        this.observerService.saveObserver('traffic_light', this.observer).subscribe(observer => this.observer = observer);
    }

    private resetObserver() {
        this.observer.condition_achieved = 0;
        this.updateObserver();
    }
}