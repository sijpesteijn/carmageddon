import {
    AfterViewInit, Component, ElementRef, EventEmitter, HostListener, Input, Output, ViewChild
} from '@angular/core';
import { Roi } from '../../observers/observer.service';

interface Point {
    x: number;
    y: number;
}

@Component({
    selector: 'stream',
    template: require('./stream.html'),
    styles: [require('./stream.scss')]
})
export class StreamComponent {
    private reloadInterval: any;
    private last_update: Date;
    private status: string = 'loading';
    @Input('roi')
    set theRoi(roi: Roi) {
        // console.log(roi);
        if (roi) {
            this.roi = roi;
            this.setRoi();
        }
    }
    @Input('interval')
    set interval(interval: number) {
        this._interval = interval;
        clearInterval(this.reloadInterval);
        this.reloadInterval = undefined;
        this.startReload();
        // setTimeout(() => this.startReload(), 500);
    }
    private _interval = 5;
    private roi: Roi;
    @ViewChild('roi') roiElem;
    @Output('onChange') changeEmitter = new EventEmitter();
    private streamDefault = '/assets/img/testbeeld.png';
    private streamUrl = this.streamDefault;
    private mouseDown = false;

    @HostListener('mousedown', ['$event'])
    onMouseDown(event: any) {
        this.mouseDown = true;
        const point = this.getPoint(event);
        this.roi = {
            x: point.x,
            y: point.y,
            width: 0,
            height: 0
        };
        this.setRoi();
    }

    @HostListener('mouseup', ['$event'])
    onMouseUp(event: any) {
        this.mouseDown = false;
        this.changeEmitter.emit(this.roi);
    }

    @HostListener('mousemove', ['$event'])
    onMouseMove(event: any) {
        if (this.mouseDown) {
            const point = this.getPoint(event);
            this.roi.width  = point.x - this.roi.x;
            this.roi.height = point.y - this.roi.y;
            this.setRoi();
        }
    }

    @HostListener('ondragstart', ['$event'])
    onDragstart(event: any) {
        return false;
    }

    ngAfterViewInit() {
        this.getWebcamImage();
        this.startReload();
    }

    private setRoi() {
        if (this.roiElem) {
            this.roiElem.nativeElement.style.top    = this.roi.y + 'px';
            this.roiElem.nativeElement.style.height = this.roi.height + 'px';
            this.roiElem.nativeElement.style.left   = this.roi.x + 'px';
            this.roiElem.nativeElement.style.width  = this.roi.width + 'px';
        }
    }

    private getPoint(event: any): Point {
        const ev = event || window.event; //Moz || IE
        const point = { x: 0, y: 0};
        if (ev.pageX) { //Moz
            point.x = ev.layerX;
            point.y = ev.layerY;
        } else if (ev.clientX) { //IE
            point.x = ev.clientX + document.body.scrollLeft;
            point.y = ev.clientY + document.body.scrollTop;
        }
        return point;
    }

    private startReload() {
        console.log('Interval: ', this._interval * 1000);
        this.reloadInterval = setInterval(() => {
            this.getWebcamImage();
        }, (this._interval * 1000));
    }

    private getWebcamImage() {
        this.last_update = new Date();
        const url = '/camera/snapshot/' + this.last_update;
        const img = new Image();
        img.onload = () => {
            this.status = 'loaded';
            this.streamUrl = url;
        };
        img.onerror = () => {
            this.status = 'error';
            this.streamUrl = this.streamDefault;
        };
        img.src = url;
    }
}