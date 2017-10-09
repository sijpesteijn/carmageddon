import {Component} from "@angular/core";
import {Car, CarService} from "../car.service";

@Component({
    selector: 'car-status',
    template: require('./car-status.html'),
    styles: [require('./car-status.scss')]
})
export class CarStatusComponent {
    private car: Car;

    constructor(private carService: CarService) {

    }

    ngOnInit() {
        this.carService.getCar().subscribe(car => {
           this.car = car;
        });
    }
}