import { TestBed } from '@angular/core/testing';

import { ImageManipulationService } from './image-manipulation.service';

describe('ImageManipulationService', () => {
  beforeEach(() => TestBed.configureTestingModule({}));

  it('should be created', () => {
    const service: ImageManipulationService = TestBed.get(ImageManipulationService);
    expect(service).toBeTruthy();
  });
});
