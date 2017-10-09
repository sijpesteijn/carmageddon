import { Component } from '@angular/core';
import {CarService} from "../car.service";
@Component({
    selector: 'manual',
    template: require('./manual.html'),
    styles: [require('./manual.scss')]
})
export class ManualComponent {

    constructor(private carService: CarService) {}

    onChange(event: any) {
        this.carService.setEngine(event.deltaY).subscribe();
        this.carService.setSteer(event.deltaX).subscribe();
    }
}